/*
 * Copyright (c) 2001 Silicon Graphics, Inc.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 * 
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 * 
 * http://www.sgi.com 
 * 
 * For further information regarding this notice, see: 
 * 
 * http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
 */

/* 
 *
 * chacl - change/delete/list file Access Control List
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <acl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

static int acl_delete_file (const char * path, acl_type_t type);
static int list_acl(char *file);
static int set_acl(acl_t acl, acl_t dacl, const char *fname);
static int walk_dir(acl_t acl, acl_t dacl, const char *fname);

static char *program;
static int rflag;

static void
usage(void)
{
	fprintf(stderr, "%s: usage: \n", program);
	fprintf(stderr, "\t%s acl pathname ...\n", program);
	fprintf(stderr, "\t%s -d dacl pathname ...\n", program);
	fprintf(stderr, "\t%s -b acl dacl pathname ...\n", program);
	fprintf(stderr, "\t%s -R pathname ...\n", program);
	fprintf(stderr, "\t%s -D pathname ...\n", program);
	fprintf(stderr, "\t%s -B pathname ...\n", program);
	fprintf(stderr, "\t%s -l pathname ...\n", program);
	fprintf(stderr, "\t%s -r pathname ...\n", program);
	exit(1);
}

int
main(int argc, char *argv[])
{
	const char *inv_acl = "%s: \"%s\" is an invalid ACL specification.\n";
	char *file;
	int switch_flag = 0;            /* ensure only one switch is used */
	int args_required = 1;	
	int failed = 0;			/* exit status */
	int c;				/* For use by getopt(3) */
	int dflag = 0;			/* a Default ACL is desired */
	int bflag = 0;			/* a both ACLs are desired */
	int Rflag = 0;			/* set to true to remove an acl */
	int Dflag = 0;			/* set to true to remove default acls */
	int Bflag = 0;			/* set to true to remove both acls */
	int lflag = 0;			/* set to true to list acls */
	acl_t acl = NULL;		/* File ACL */
	acl_t dacl = NULL;		/* Directory Default ACL */

	program = basename(argv[0]);

	acl_set_compat(ACL_COMPAT_IRIXGET);

	/* parse arguments */
	while ((c = getopt(argc, argv, "bdlRDBr")) != -1) {
		if (switch_flag) 
			usage();
		switch_flag = 1;

		switch (c) {
			case 'b':
				bflag = 1;
				args_required = 3;
				break;
			case 'd':
				dflag = 1;
				args_required = 2;
				break;
			case 'R':
				Rflag = 1;
				break;
			case 'D':
				Dflag = 1;
				break;
			case 'B':
				Bflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case 'r':
				rflag = 1;
				break;
			default:
				usage();
				break;
		}
	}

	/* if not enough arguments quit */
	if ((argc - optind) < args_required)
		usage();

        /* list the acls */
	if (lflag) {
		for (; optind < argc; optind++) {	
			file = argv[optind];
			if (!list_acl(file))
				failed++;
		}
		return(failed);
	}

	/* remove the acls */
	if (Rflag || Dflag || Bflag) {
		for (; optind < argc; optind++) {	
			file = argv[optind];
			if (!Dflag &&
			    (acl_delete_file(file, ACL_TYPE_ACCESS) == -1)) {
				fprintf(stderr,
			"%s: error removing access acl on \"%s\": %s\n",
					program, file, strerror(errno));
				failed++;
			}
			if (!Rflag &&
			    (acl_delete_file(file, ACL_TYPE_DEFAULT) == -1)) {
				fprintf(stderr,
			"%s: error removing default acl on \"%s\": %s\n", 
					program, file, strerror(errno));
				failed++;
			}
		}
		return(failed);
	} 

	/* file access acl */
	if (! dflag) { 
		acl = acl_from_text(argv[optind]);
		if (acl == NULL || acl_valid(acl) == -1) {
			fprintf(stderr, inv_acl, program, argv[optind]);
			return(1);
		} 
		optind++;
	}


	/* directory default acl */
	if (bflag || dflag) {
		dacl = acl_from_text (argv[optind]);
		if (dacl == NULL || acl_valid (dacl) == -1) {
			fprintf(stderr, inv_acl, program, argv[optind]);
			return(1);
		}
		optind++;
	}

	/* place acls on files */
	for (; optind < argc; optind++)
		failed += set_acl(acl, dacl, argv[optind]);

	if (acl)
		acl_free(acl);
	if (dacl)
		acl_free(dacl);

	return(failed);
}

/* 
 *   deletes an access acl or directory default acl if one exists
 */ 
static int 
acl_delete_file(const char *path, acl_type_t type)
{
	struct acl acl;
	int error = 0;

	acl.acl_cnt = ACL_NOT_PRESENT;

	error = acl_set_file(path, type, &acl);

	return(error);
}

/*
 *    lists the acl for a file/dir in short text form
 *    return 0 on failure
 *    return 1 on success
 */
static int
list_acl(char *file)
{
	acl_t acl = NULL;
	acl_t dacl = NULL;
	char *buf_acl = NULL;
	char *buf_dacl = NULL;

	if ((acl = acl_get_file(file, ACL_TYPE_ACCESS)) == NULL) {
		fprintf(stderr, "%s: error getting ACL on \"%s\": %s\n",
			program, file, strerror(errno));
		return 0;
	}
        if (acl->acl_cnt != ACL_NOT_PRESENT) {
		buf_acl = acl_to_short_text(acl, (ssize_t *) NULL);
		if (buf_acl == NULL) {
			fprintf(stderr,
		"%s: error converting ACL to short text for file \"%s\": %s\n",
				program, file, strerror(errno));
			return 0;
		}
	}

	if ((dacl = acl_get_file(file, ACL_TYPE_DEFAULT)) == NULL) {
		fprintf(stderr, "%s: error getting default ACL on \"%s\": %s\n",
			program, file, strerror(errno));
		return 0;
	}
	if (dacl->acl_cnt > 0) {
		buf_dacl = acl_to_short_text(dacl, (ssize_t *) NULL);
		if (buf_dacl == NULL) {
			fprintf(stderr,
	"%s: error converting default ACL to short text for file \"%s\": %s\n",
				program, file, strerror(errno));
			return 0;
	    	}
	}

	printf("%s [", file);
	if (buf_acl)
		printf("%s", buf_acl);
	if (buf_dacl)
		printf("/%s", buf_dacl);
	printf("]\n");

        if (acl)
	    acl_free(acl);
        if (dacl)
	    acl_free(dacl);
        if (buf_acl)
	    acl_free(buf_acl);
        if (buf_dacl)
	    acl_free(buf_dacl);
      
        return(1);
}

static int
set_acl(acl_t acl, acl_t dacl, const char *fname)
{
	int failed = 0;

	if (rflag)
		failed += walk_dir(acl, dacl, fname);

	/* set regular acl */
	if (acl && acl_set_file(fname, ACL_TYPE_ACCESS, acl) == -1) {
		fprintf(stderr,"%s: error setting access acl on \"%s\": %s\n",
			 program, fname, strerror(errno));
		failed++;
	}
	/* set default acl */
	if (dacl && acl_set_file(fname, ACL_TYPE_DEFAULT, dacl) == -1) {
		fprintf(stderr,"%s: error setting default acl on \"%s\": %s\n",
			program, fname, strerror(errno));
		failed++;
	}

	return(failed);
}

static int
walk_dir(acl_t acl, acl_t dacl, const char *fname)
{
	int failed = 0;
	DIR *dir;
	struct dirent64 *d;
	char *name;

	if ((dir = opendir(fname)) == NULL) {
		if (errno != ENOTDIR) {
			fprintf(stderr, "%s: opendir failed: %s\n",
				program, strerror(errno));
			return(1);
		}
		return(0);	/* got a file, not an error */
	}

	while ((d = readdir64(dir)) != NULL) {
		/* skip "." and ".." entries */
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
			continue;
		
		name = malloc(strlen(fname) + strlen(d->d_name) + 2);
		if (name == NULL) {
			fprintf(stderr, "%s: malloc failed: %s\n",
				program, strerror(errno));
			exit(1);
		}
		sprintf(name, "%s/%s", fname, d->d_name);

		failed += set_acl(acl, dacl, name);
		free(name);
	}
	closedir(dir);

	return(failed);
}