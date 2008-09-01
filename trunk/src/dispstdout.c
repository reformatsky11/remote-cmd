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

void DispStdout( LIBSSH2_CHANNEL *channel ) {
	char *buf, *cmdout;
	int num, test_err;
	int timeout = 100;
	int nfds = 1;
	LIBSSH2_POLLFD *fds;
	size_t bufsiz = 8193, ret = 0;
	
	if ((buf = malloc (bufsiz)) == NULL)
		return;
	
	test_err = 1;
	if (set_non_blocking == 1) {
		/* poll for stdout */
		if (( fds = malloc (sizeof (LIBSSH2_POLLFD) * 2)) == NULL) 
			return;
		fds[0].type = LIBSSH2_POLLFD_CHANNEL;
		fds[0].fd.channel = channel;
		fds[0].events = LIBSSH2_POLLFD_POLLIN;
		fds[0].revents = LIBSSH2_POLLFD_POLLIN;
		
		test_err = libssh2_poll (fds, nfds, timeout);
		
		free (fds);
	}
	
	ret = 0;
	if (test_err > 0) {
		/* Get STDOUT */
		ret = libssh2_channel_read (channel, buf, bufsiz - 10);
	}
	if (ret > 0) {
		if ((cmdout = malloc (ret + 1)) != NULL) {
			num = snprintf (cmdout, ret + 1, "%s", buf);
			if (verbose)
				fprintf (stdout, "STDOUT:\n%s\n", cmdout);
		}
	}
	free (buf);
	
	return;
}

