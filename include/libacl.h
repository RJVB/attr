/*
  File: libacl.h

  (C) 1999, 2000 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

#ifndef __ACL_LIBACL_H
#define __ACL_LIBACL_H

#include <sys/acl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Flags for acl_to_any_text() */

/* Print NO, SOME or ALL effective permissions comments. SOME prints
   effective rights comments for entries which have different permissions
   than effective permissions.  */
#define TEXT_SOME_EFFECTIVE		0x01
#define TEXT_ALL_EFFECTIVE		0x02

/* Align effective permission comments to column 32 using tabs or
   use a single tab. */
#define TEXT_SMART_INDENT		0x04

/* User and group IDs instead of names. */
#define TEXT_NUMERIC_IDS		0x08

/* Only output the first letter of entry types
   ("u::rwx" instead of "user::rwx"). */
#define TEXT_ABBREVIATE			0x10

/* acl_check error codes */

#define ACL_MULTI_ERROR		(0x1000)     /* multiple unique objects */
#define ACL_DUPLICATE_ERROR	(0x2000)     /* duplicate Id's in entries */
#define ACL_MISS_ERROR		(0x3000)     /* missing required entry */
#define ACL_ENTRY_ERROR		(0x4000)     /* wrong entry type */

extern char *acl_to_any_text(acl_t acl, const char *prefix,
			     char separator, int options);
extern int acl_cmp(acl_t acl1, acl_t acl2);
extern int acl_check(acl_t acl, int *last);
extern acl_t acl_from_mode(mode_t mode);
extern int acl_equiv_mode(acl_t acl, mode_t *mode_p);
int acl_extended_file(const char *path_p);
int acl_extended_fd(int fd);
extern int acl_entries(acl_t acl);
extern const char *acl_error(int code);
extern int acl_get_perm(acl_permset_t permset_d, acl_perm_t perm);

#ifdef __cplusplus
}
#endif

#endif  /* __ACL_LIBACL_H */
