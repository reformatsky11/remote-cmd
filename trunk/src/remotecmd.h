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

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pwd.h>
#include <regex.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <unistd.h>

#define SSH_PORT 22
#define MAXLEN   8192
#define AUTHOR   "Mike Watters <valholla75@gmail.com>"

LIBSSH2_SESSION   *session;
char              *username, *password, *appname, *cmderr;
char              *errfile, *envfilenm, *termtype;
char              rsapubkey[MAXLEN], rsaprivkey[MAXLEN];
char              dsapubkey[MAXLEN], dsaprivkey[MAXLEN];
char              idpubkey[MAXLEN], idprivkey[MAXLEN];
int               sock, debug, verbose, errpause, skiperr, allocpty, envfl;
int               tracelvl, enable_trace, set_non_blocking;
struct termios    saved_attributes;
unsigned int      timeout;

char * trim( char [] );
char * GetValue( char [] );
int CreateSocket( unsigned long );
int CheckLine( char [] );
int SkipComments( char [] );
int SkipError( char * );
unsigned long GetHostInfo( char * );
void AuthHost( void );
void CleanupSession( LIBSSH2_SESSION *, char * );
void CloseChannel( LIBSSH2_CHANNEL * );
void DispStdout( LIBSSH2_CHANNEL * );
void DispStderr( LIBSSH2_CHANNEL * );
void Echo_off( void );
void Echo_on( void );
void EntertoContinue( void );
void FreeChannel( LIBSSH2_CHANNEL * );
void GetUserInfo( void );
void InitSession( unsigned long );
void PrintUsage( void );
void RunCommand( char * );
void SetEnv( LIBSSH2_CHANNEL * );
void ValidateOptions( char *, char * );

