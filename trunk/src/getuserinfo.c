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

void GetUserInfo() {
	char *homedir = NULL;
	struct passwd *pw;
	uid_t uid = geteuid();
	
	setpwent();
	pw = getpwuid(uid);
	if (pw != NULL) {
		username = pw->pw_name;
		homedir = pw->pw_dir;
	}
	endpwent();
	
	/* Get RSA Key info from effecive user id */
	sprintf(rsapubkey,"%s/.ssh/id_rsa.pub",homedir);
	sprintf(rsaprivkey,"%s/.ssh/id_rsa",homedir);
	sprintf(dsapubkey,"%s/.ssh/id_dsa.pub",homedir);
	sprintf(dsaprivkey,"%s/.ssh/id_dsa",homedir);
	sprintf(idpubkey,"%s/.ssh/identity.pub",homedir);
	sprintf(idprivkey,"%s/.ssh/identity",homedir);
	if (debug) {
		printf ("**DEBUG** RSA PUBKEY = %s\n", rsapubkey);
		printf ("**DEBUG** RSA PRIVKEY = %s\n", rsaprivkey);
	}
	
	return;
}

