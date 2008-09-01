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

void InitSession( unsigned long hostaddr ) {
	sock = CreateSocket(hostaddr);
	session = libssh2_session_init ();
	
	/* startup SSH session on socket */
	if ( libssh2_session_startup(session, sock) ) {
		fprintf (stderr, "Failure establishing SSH session\n");
		exit(-1);
	}
	
	/* Enable Tracing if specified by user */
	if (enable_trace)
		libssh2_trace (session, tracelvl);
	
	AuthHost();
	
	return;
}

