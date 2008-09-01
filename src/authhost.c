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

void AuthHost() {
	char *error;
	char prompt[80];
	char passenter[80];
	
	/* Auth by public key */
	if (libssh2_userauth_publickey_fromfile
		(session, username, rsapubkey, rsaprivkey, password)) {
			/* public key authentication failed
			 Attempt to authenticate via password */
			sprintf(prompt,"Enter Password for %s: ",username);
			strncpy(passenter, getpass(prompt),80);
			password=trim(passenter);
			if (libssh2_userauth_password(session, username, password)) {
				fprintf(stderr,"Authentication failed.\n");
				libssh2_session_last_error(session, &error, NULL, 0);
				if (debug)
					fprintf(stderr,"libssh2_userauth_password: %s\n",error);
				CleanupSession(session,username);
			}
		}
	return;
}

