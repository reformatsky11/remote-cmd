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

void CleanupSession( LIBSSH2_SESSION *session, char *username ) {
	char disconnect_msg[MAXLEN];
	
	sprintf(disconnect_msg,"Remote commad automation script run by %s completed",
			username);
	libssh2_session_disconnect (session, disconnect_msg);
	libssh2_session_free (session);
	sleep (1); /* sleep to give session time to close */
	close(sock);
	exit(0);
}
