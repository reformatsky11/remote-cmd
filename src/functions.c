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

char * trim(char str[]) {
	int i=0,j;
	
	/* Trim spaces and tabs from beginning */
	if (strlen(str) < 1) {
		sprintf(str,"%s","N/A");
		return(str);
	}
	while((str[i]==' ')||(str[i]=='\t')) {
		i++;
	}
	if(i>0) {
		for(j=0;j<strlen(str);j++) {
			str[j]=str[j+i];
		}
		str[j]='\0';
	}
	
	/* Trim spaces, tabs, and Carriage returns from end */
	i=strlen(str)-1;
	while((str[i]==' ')||(str[i]=='\t') || 
		  (str[i]=='\n') || (str[i]=='\r')) {
			  i--;
		  }
	if(i<(strlen(str)-1)) {
		str[i+1]='\0';
	}
	
	return(str);
}

int SkipComments( char str[] ) {
	
	if ( toascii(str[0]) == '#' ) {
		return 0;
	} else {
		return 1;
	}
	return 0;
}

int CheckLine( char str[] ) {
	int i;
	i = strlen(str)-1;
	if ( toascii(str[i]) == '\\' ) {
		str[i] = '\0';
		return 1;
	} else {
		return 0;
	}
	return 0;
}

void ValidateOptions(char *cmdline, char *filename) {
	
	if ( cmdline == NULL && filename == NULL )
		PrintUsage();
	if ( cmdline != NULL && filename != NULL )
		PrintUsage();
	if (cmdline != NULL && toascii(cmdline[0]) == '-')
		PrintUsage();
	if (filename != NULL && toascii(filename[0]) == '-')
		PrintUsage();
	if (skiperr > 0 && toascii(errfile[0]) == '-') 
		PrintUsage();
	if (envfl > 0 && toascii(envfilenm[0]) == '-') 
		PrintUsage();
	return;
}

void PrintUsage( void ) {
	fprintf(stderr, "Usage: %s [-d] [-p] [-e {envfile}] [-v] [-P] [-T n] [-s {errfile}] [-t n] {-c command | -f filename} {hostname}\n",appname);
	fprintf(stderr, "\t-d  turns on DEBUG mode\n");
	fprintf(stderr, "\t\t  this will show the command that was run\n");
	fprintf(stderr, "\t-h  this usage statement\n");
	fprintf(stderr, "\t-p  pause on errors\n");
	fprintf(stderr, "\t\t  this will cause the app to wait\n");
	fprintf(stderr, "\t\t  on the user when errors are encountered\n");
	fprintf(stderr, "\t-e  Use {envfile} to set remote environment\n");
	fprintf(stderr, "\t-v  turns on VERBOSE mode\n");
	fprintf(stderr, "\t\t  without -v you will only see output on an error\n");
	fprintf(stderr, "\t-T  This will enable tracing where 'n' is the trace level\n");
	fprintf(stderr, "\t-P  This will Allocate a controlling PTY for the session\n");
	fprintf(stderr, "\t-s  Skips command errors that meet regexp(s) in {errfile}\n");
	fprintf(stderr, "\t-t  n is connection timeout in seconds DEFAULT 5 seconds\n");
	fprintf(stderr, "\t-c  command to run on the remote host\n");
	fprintf(stderr, "\t-f  filename containing commands to run on the remote host\n");
	fprintf(stderr, "\t\t -c and -f are mutually exclusive\n");
	fprintf(stderr, "\t   {hostname} remote host to run command(s) on\n");
	fprintf(stderr, "\t\t default to 127.0.0.1 if not specified\n");
	exit(255);
	return;
}

void EntertoContinue() {
	char c;
	
	while (c != '\n') {
		read (STDIN_FILENO, &c, 1);
	}
	return;
}

char * GetValue( char line[] ) {
	char delims[] = "\"=#";
	char *result = NULL;
	
	result = strtok( NULL, delims );
	if (result == NULL) {
		result = "N/A";
	}
	return(result);
}

