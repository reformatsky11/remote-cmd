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

void DispStderr( LIBSSH2_CHANNEL *channel ) {
	char *buf;
	int num, test_err;
	int nfds = 1;
	int timeout = 100;
	size_t bufsiz = 8193, ret = 0;
	LIBSSH2_POLLFD *fdserr;
	
	test_err = 0;
	if (set_non_blocking == 1) {
		/* poll for stderr */
		if ((fdserr = malloc (sizeof (LIBSSH2_POLLFD) * 2)) == NULL)
			return;
		fdserr[0].type = LIBSSH2_POLLFD_CHANNEL;
		fdserr[0].fd.channel = channel;
		fdserr[0].events = LIBSSH2_POLLFD_POLLEXT;
		fdserr[0].revents = LIBSSH2_POLLFD_POLLEXT;
		
		test_err = libssh2_poll (fdserr, nfds, timeout);
		
		free(fdserr);
	} else {
		test_err = 1;
	}
	
	if (test_err > 0) {
		ret = 0;
		if ((buf = malloc (bufsiz)) == NULL)
			return;
		ret = libssh2_channel_read_stderr (channel, buf, bufsiz - 10);
		
		if (ret > 0) {
			if ((cmderr = malloc (ret + 1)) != NULL) {
				num = snprintf (cmderr, ret + 1, "%s", buf);
			}
		}
		
		free (buf);
	}
	return;
}
