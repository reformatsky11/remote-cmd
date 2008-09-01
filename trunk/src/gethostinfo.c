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

unsigned long GetHostInfo( char *host) {
	unsigned long hostaddress = 0;
	in_addr_t addr;
	char **p;
	struct hostent *hp;
	
	if ((int)(addr = inet_addr(host)) == -1) {
		hp = gethostbyname((char *) host);
	} else {
		hostaddress = inet_addr(host);
		return hostaddress;
	}
	if (hp == NULL) {
		fprintf(stderr, "Host \"%s\" not found\n", host);
		exit(-1);
	}
	for (p = hp->h_addr_list; *p != 0; p++) {
		struct in_addr in;
		memcpy(&in.s_addr, *p, sizeof (in.s_addr));
		hostaddress = inet_addr(inet_ntoa(in));
	}
	
	return hostaddress;
}

