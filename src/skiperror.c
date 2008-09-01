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

int SkipError(char *cmderr) {
	FILE *errfl;
	char line[MAXLEN];
	char *errtok = NULL;
	regex_t regex;
	size_t num_fnd = 0;
	
	if ( skiperr > 0 ) {
		if ( (errfl = fopen(errfile, "r")) == 0 ) {
			fprintf(stderr, "Could not open skipFile \"%s\":%s\n", 
					errfile, strerror(errno));
			exit(-1);
		}
		while ( fgets(line, sizeof(line), errfl) != NULL ) {
			errtok = strdup(line);
			trim(errtok);
			
			if ( SkipComments(errtok) && strlen(errtok) > 1 ) {
				if ( regcomp(&regex, errtok, REG_EXTENDED|REG_ICASE|REG_NOSUB) != 0) {
					fprintf(stderr,"Invalid Regular Expression: \"%s\\n",errtok);
					free(errtok);
					regfree(&regex);
					free(&regex);
					exit(-1);
				}
				if ( regexec(&regex, cmderr, num_fnd, NULL, 0) == 0 ) {
					free(errtok);
					regfree(&regex);
					free(&regex);
					return 1;
				}
				regfree(&regex);
				free(&regex);
			}
			free(errtok);
		}
	}
	return 0;
}

