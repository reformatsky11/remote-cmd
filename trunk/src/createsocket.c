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

int CreateSocket( unsigned long hostaddr ) {
	int sockopt, sock;
	struct sockaddr_in sin;
	long arg;
	fd_set mysock;
	struct timeval tv;
	socklen_t lon;
	
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	/* Set non-blocking */
	arg = fcntl(sock, F_GETFL, NULL);
	fcntl(sock, F_SETFL, arg | O_NONBLOCK);
	
	/* Trying to connect with timeout */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SSH_PORT);
	sin.sin_addr.s_addr = hostaddr;
	if ((connect(sock, (struct sockaddr *)&sin,
				 sizeof(struct sockaddr_in))) < 0) {
					 if (errno == EINPROGRESS) {
						 tv.tv_sec = timeout;
						 tv.tv_usec = 0;
						 FD_ZERO(&mysock);
						 FD_SET(sock, &mysock);
						 
						 if ( (select(sock+1, &mysock, &mysock, 
									  &mysock, &tv)) > 0 ) {
							 lon = sizeof(int);
							 getsockopt(sock, SOL_SOCKET, SO_ERROR, 
										(void*)(&sockopt), &lon);
							 if (sockopt) {
								 fprintf(stderr, 
										 "Error connecting to \"%s\": %s\n",
										 inet_ntoa(sin.sin_addr), 
										 strerror(sockopt));
								 exit(-1);
							 }
						 } else {
							 fprintf(stderr, "Failed to connect to \"%s\" \
									 within the %d second(s) timeout.\n", 
									 inet_ntoa(sin.sin_addr), timeout);
							 exit(-1);
						 }
					 } else {
						 fprintf(stderr, "Error connecting to \"%s\": %s\n", 
								 inet_ntoa(sin.sin_addr), strerror(errno));
						 exit(-1);
					 }
				 }
	/* Set to blocking mode again... */
	fcntl(sock, F_SETFL, 0);
	
	return sock;
}

