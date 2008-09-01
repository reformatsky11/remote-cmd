/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
	 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include "remotecmd.h"

void SetEnv (LIBSSH2_CHANNEL *channel) {
	FILE *envfile;
	char *envtok = NULL;
	char line[MAXLEN];
	char tmpenv[MAXLEN];
	char delims[] = "\"\'=#";
	char *def_path = "/usr/bin:/usr/local/bin";
	char *def_editor = "/bin/vi";
	char *def_term = "vt100";
		
	if ( (envfile = fopen(envfilenm, "r")) == 0 ) {
		if (debug)
			printf("**DEGUG** Setting Default Environment\n");
		
		libssh2_channel_setenv (channel, "PATH", def_path);
		libssh2_channel_setenv (channel, "EDITOR", def_editor);
		libssh2_channel_setenv (channel, "TERM", def_term);
		return;
	}
	
	if (debug) {
		printf("**DEGUG** Setting Custom Environment\n");
		printf("**DEGUG** using %s\n", envfilenm);
	}
	strcpy(tmpenv,"");
	while ( fgets(line, sizeof(line), envfile) != NULL ) {
		envtok = strdup(line);
		if ( CheckLine(trim(envtok)) ) {
			sprintf(tmpenv,"%s%s",tmpenv,envtok);
		} else {
			if ( strcmp(tmpenv,"") != 0 ) {
				sprintf(tmpenv,"%s%s",tmpenv,envtok);
				free(envtok);
				envtok = strdup(tmpenv);
			}
			if (strlen(envtok) > 1 && SkipComments(envtok) ) {
				libssh2_channel_setenv(channel, strtok(envtok, delims), 
									   GetValue(envtok));
			}
			strcpy(tmpenv,"");
		}
		free(envtok);
	}
	if ( strcmp(tmpenv,"") != 0 ) {
		libssh2_channel_setenv(channel, strtok(envtok, delims), 
							   GetValue(envtok));
		strcpy(tmpenv,"");
	}
	return;
}

