/*
  File: user_group.c
  (Linux Access Control List Management)

  Copyright (C) 1999, 2000
  Andreas Gruenbacher, <a.gruenbacher@computer.org>
 	
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdlib.h>
#include <stdio.h>
#include "user_group.h"


const char *
user_name(
	uid_t uid)
{
	struct passwd *passwd = getpwuid(uid);
	static char uid_str[12];
	int ret;

	if (passwd != NULL)
		return passwd->pw_name;
	ret = snprintf(uid_str, sizeof(uid_str), "%ld", (long)uid);
	if (ret < 1 || ret >= sizeof(uid_str))
		return "?";
	return uid_str;
}


const char *
group_name(
	gid_t gid)
{
	struct group *group = getgrgid(gid);
	static char gid_str[12];
	int ret;

	if (group != NULL)
		return group->gr_name;
	ret = snprintf(gid_str, sizeof(gid_str), "%ld", (long)gid);
	if (ret < 1 || ret >= sizeof(gid_str))
		return "?";
	return gid_str;
}
