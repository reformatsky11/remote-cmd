/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) Mike Watters 2008 <valholla75@gmail.com>
 * 
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "remotecmd.h"

int main (int argc, char *argv[]) {
	int c;
	unsigned long hostaddr;
	FILE *cmdfile;
	char *cmdline = NULL;
	char *cmdtok = NULL;
	char *filename = NULL;
	char line[MAXLEN];
	char tmpcmd[MAXLEN];
	debug=0;verbose=0;errpause=0;skiperr=0;allocpty=0;envfl=0;
	timeout=5;
	enable_trace=0;
	tracelvl=0;
	errfile=NULL;
	envfilenm=NULL;
	appname = basename(argv[0]);
	while ((c = getopt (argc, argv, "dhpvP :c:f:t:s:e:T:")) != -1) {
		switch (c) {
			case 'c':
				cmdline = optarg;
				break;
				
			case 'd':
				debug++;
				break;
				
			case 'f':
				filename = optarg;
				break;
				
			case 'h':
				PrintUsage();
				break;
				
			case 'p':
				errpause++;
				break;
				
			case 'P':
				allocpty++;
				break;
				
			case 'e':
				envfl++;
				envfilenm = optarg;
				break;
				
			case 's':
				skiperr++;
				errfile = optarg;
				break;
				
			case 't':
				timeout = (int) strtol(optarg, (char **)NULL, 10);
				break;
				
			case 'T':
				enable_trace++;
				tracelvl = (int) strtol(optarg, (char **)NULL, 10);
				break;
				
			case 'v':
				verbose++;
				break;
				
			case '?':
				PrintUsage();
				break;
		}
	}
	
	ValidateOptions(cmdline,filename);
	
	if ((argc - optind) == 1) {
		hostaddr = GetHostInfo((char *)argv[optind]);
	} else {
		hostaddr = htonl (0x7F000001);
	}
	
	/* Get Username from effecive user id running the program */
	GetUserInfo();
	
	/* Initialize the Session */
	InitSession(hostaddr);
	
	if (cmdline != NULL) {
		RunCommand(cmdline);
	} else {
		if ((cmdfile = fopen(filename, "r")) == 0) {
			fprintf(stderr, "Could not open file %s:%s\n",
					filename ,strerror(errno));
			exit(-1);
		}
		
		strcpy(tmpcmd,"");
		while (fgets(line, sizeof(line), cmdfile) != NULL) {
			cmdtok = strdup(line);
			if (CheckLine(trim(cmdtok))) {
				sprintf(tmpcmd,"%s%s",tmpcmd,cmdtok);
			} else {
				if ( strcmp(tmpcmd,"") != 0 ) {
					sprintf(tmpcmd,"%s%s",tmpcmd,cmdtok);
					free(cmdtok);
					cmdtok = strdup(tmpcmd);
				}
				if (strlen(cmdtok) > 1 && SkipComments(cmdtok) ) {
					RunCommand(cmdtok);
				}
				strcpy(tmpcmd,"");
			}
			free(cmdtok);
		}
		if (strcmp(tmpcmd,"") != 0) {
			RunCommand(tmpcmd);
			strcpy(tmpcmd,"");
		}
	}
	CleanupSession(session,username);
	return 0;
}

