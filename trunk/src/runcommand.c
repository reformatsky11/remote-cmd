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

void RunCommand(char *command) {
	LIBSSH2_CHANNEL *channel;
	char *error;
	
	channel=NULL;
	cmderr=NULL;
	set_non_blocking = 1;
	
	/* Open a channel for command */
	if ( !( channel = libssh2_channel_open_session(session) ) ) {
		libssh2_session_last_error(session, &error, NULL, 0);
		fprintf (stderr, "Unable to open a channel session\n");
		if (debug)
			fprintf(stderr, "libssh2_channel_open_session: %s\n", error);
		CleanupSession(session,username);
	}
	
	/* Force Data to on STDOUT and STDERR to be on seperate channels 
	 * read individually with *_read and *_read_stderr functions */
	libssh2_channel_handle_extended_data(channel, 
										 LIBSSH2_CHANNEL_EXTENDED_DATA_NORMAL);
	/* Request a PTY on Channel */
	if (allocpty) {
		if (libssh2_channel_request_pty(channel, (char*)"vt100")) {
			fprintf(stderr, "Failed requesting pty\n");
			if (debug)
				fprintf(stderr, "libssh2_channel_request_pty: %s\n", error);
			CleanupSession(session,username);
		}
		/* Set Environment */
		SetEnv(channel);
	}
	
	if (debug)
		printf("**DEBUG** Command = %s\n", command);
	
	/* execute the command(s) */
	if ( libssh2_channel_process_startup
		(channel, "exec", 4, command, strlen(command)+1) ) {
			fprintf (stderr, "Unable to exec command:\"%s\"\n",command);
			if (debug)
				fprintf(stderr, "libssh2_channel_process_startup: %s\n", error);
			FreeChannel(channel);
			return;
		}
	
	/* set the channel to blocking/non blocking mode */
	if (set_non_blocking == 0) {
		libssh2_channel_set_blocking (channel, 1);
	} else {
		libssh2_channel_set_blocking (channel, 0);
	}
	
	/* Display STDOUT and STDERR for command */
	if (verbose)
		fprintf(stdout, "Command = %s\n", command);
	while (1) {
		DispStdout(channel);
		DispStderr(channel); 
		/* break out of polling loop */
		if (libssh2_channel_eof (channel) != 0) {
			/* Return channel to Blocking and free */
			if (set_non_blocking == 1)
				libssh2_channel_set_blocking (channel, 1);
			break;
		}
	}
	
	if (cmderr != NULL) {
		if ( SkipError(cmderr) == 0 ) {
			fprintf(stderr,"*****************************************************\n");
			fprintf(stderr,"Command \"%s\"\n",command);
			fprintf(stderr,"Failed with STDERR:\n");
			fprintf(stderr,"%s\n",cmderr);
			fprintf(stderr,"*****************************************************\n");
			if ( errpause ) {
				fprintf(stderr,"Press ENTER to continue:\n");
				EntertoContinue();
			}
		}
	}
	free(cmderr);
	cmderr=NULL;
	
	/* Return channel to Blocking and free */
	if (set_non_blocking == 1)
		libssh2_channel_set_blocking (channel, 1); 
	channel=NULL;
	CloseChannel(channel);
	return;
}

