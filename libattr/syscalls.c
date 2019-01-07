/*
  Copyright (C) 2015  Dmitry V. Levin <ldv@altlinux.org>

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * These dumb wrappers are for backwards compatibility only.
 * Actual syscall wrappers are long gone to libc.
 */

#include "config.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/xattr.h>

#ifdef HAVE_VISIBILITY_ATTRIBUTE
# pragma GCC visibility push(default)
#endif

#ifdef __APPLE__
    static ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size)
    {
        return getxattr(path, name, value, size, 0, XATTR_SHOWCOMPRESSION|XATTR_NOFOLLOW);
    }
    static ssize_t _getxattr(const char *path, const char *name, void *value, size_t size)
    {
        return getxattr(path, name, value, size, 0, XATTR_SHOWCOMPRESSION);
    }
    static int lsetxattr (const char *path, const char *name, const void *value, size_t size, int flags)
    {
        return setxattr(path, name, value, size, 0, flags|XATTR_SHOWCOMPRESSION|XATTR_NOFOLLOW);
    }
    static int lremovexattr (const char *path, const char *name)
    {
        return removexattr(path, name, XATTR_SHOWCOMPRESSION|XATTR_NOFOLLOW);
    }
    static ssize_t llistxattr(const char *path, char *list, size_t size)
    {
        return listxattr(path, list, size, XATTR_SHOWCOMPRESSION|XATTR_NOFOLLOW);
    }

    #define fgetxattr(fd,n,v,s)     fgetxattr((fd),(n),(v),(s),0,XATTR_SHOWCOMPRESSION)
    #define getxattr(p,n,v,s)       _getxattr((p),(n),(v),(s))
    #define fsetxattr(fd,n,v,s,f)   fsetxattr((fd),(n),(v),(s),0,(f)|XATTR_SHOWCOMPRESSION)
    #define setxattr(p,n,v,s,f)     setxattr((p),(n),(v),(s),0,(f)|XATTR_SHOWCOMPRESSION)
    #define fremovexattr(fd,n)      fremovexattr((fd),(n),XATTR_SHOWCOMPRESSION)
    #define removexattr(p,n)        removexattr((p),(n),XATTR_SHOWCOMPRESSION)
    #define flistxattr(fd,l,s)      flistxattr((fd),(l),(s),XATTR_SHOWCOMPRESSION)
    #define listxattr(p,l,s)        listxattr((p),(l),(s),XATTR_SHOWCOMPRESSION)
#endif

int libattr_setxattr(const char *path, const char *name,
		     void *value, size_t size, int flags)
{
#ifdef linux
	return syscall(__NR_setxattr, path, name, value, size, flags);
#else
	return setxattr(path, name, value, size, flags);
#endif
}

int libattr_lsetxattr(const char *path, const char *name,
		      void *value, size_t size, int flags)
{
#ifdef linux
	return syscall(__NR_lsetxattr, path, name, value, size, flags);
#else
	return lsetxattr(path, name, value, size, flags);
#endif
}

int libattr_fsetxattr(int filedes, const char *name,
		      void *value, size_t size, int flags)
{
#ifdef linux
	return syscall(__NR_fsetxattr, filedes, name, value, size, flags);
#else
	return fsetxattr(filedes, name, value, size, flags);
#endif
}

ssize_t libattr_getxattr(const char *path, const char *name,
			 void *value, size_t size)
{
#ifdef linux
	return syscall(__NR_getxattr, path, name, value, size);
#else
	return getxattr(path, name, value, size);
#endif
}

ssize_t libattr_lgetxattr(const char *path, const char *name,
			  void *value, size_t size)
{
#ifdef linux
	return syscall(__NR_lgetxattr, path, name, value, size);
#else
	return lgetxattr(path, name, value, size);
#endif
}

ssize_t libattr_fgetxattr(int filedes, const char *name,
			  void *value, size_t size)
{
#ifdef linux
	return syscall(__NR_fgetxattr, filedes, name, value, size);
#else
	return fgetxattr(filedes, name, value, size);
#endif
}

ssize_t libattr_listxattr(const char *path, char *list, size_t size)
{
#ifdef linux
	return syscall(__NR_listxattr, path, list, size);
#else
	return listxattr(path, list, size);
#endif
}

ssize_t libattr_llistxattr(const char *path, char *list, size_t size)
{
#ifdef linux
	return syscall(__NR_llistxattr, path, list, size);
#else
	return llistxattr(path, list, size);
#endif
}

ssize_t libattr_flistxattr(int filedes, char *list, size_t size)
{
#ifdef linux
	return syscall(__NR_flistxattr, filedes, list, size);
#else
	return flistxattr(filedes, list, size);
#endif
}

int libattr_removexattr(const char *path, const char *name)
{
#ifdef linux
	return syscall(__NR_removexattr, path, name);
#else
	return removexattr(path, name);
#endif
}

int libattr_lremovexattr(const char *path, const char *name)
{
#ifdef linux
	return syscall(__NR_lremovexattr, path, name);
#else
    return lremovexattr(path, name);
#endif
}

int libattr_fremovexattr(int filedes, const char *name)
{
#ifdef linux
	return syscall(__NR_fremovexattr, filedes, name);
#else
	return fremovexattr(filedes, name);
#endif
}

#ifdef HAVE_VISIBILITY_ATTRIBUTE
# pragma GCC visibility pop
#endif
