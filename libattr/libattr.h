/* Features we provide ourself. */

#define HAVE_ATTR_LIBATTR_H 1
#define HAVE_CONFIG_H 1

#define HAVE_FGETXATTR 1
#define HAVE_FLISTXATTR 1
#define HAVE_FSETXATTR 1
#define HAVE_GETXATTR 1
#define HAVE_LISTXATTR 1
#define HAVE_SETXATTR 1
#ifndef __APPLE__
    #define HAVE_LGETXATTR 1
    #define HAVE_LLISTXATTR 1
    #define HAVE_LSETXATTR 1
#endif
