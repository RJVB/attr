# Copyright (C) 2009  Andreas Gruenbacher <agruen@suse.de>
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

AC_INIT([attr], [2.4.48], [acl-devel@nongnu.org])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([include/config.h])
AC_PREFIX_DEFAULT(/usr)

AM_INIT_AUTOMAKE([-Wall foreign 1.11 dist-xz subdir-objects])
AM_SILENT_RULES([yes])

AC_CANONICAL_HOST
AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS
AC_C_CONST
AC_TYPE_MODE_T
AC_FUNC_ALLOCA
AC_FUNC_GCC_VISIBILITY
AC_SYS_LARGEFILE

has_version_script=no

# Detect the target system
case "${host_os}" in
    linux*)
        has_version_script=yes
        ;;
esac

# Pass the conditionals to automake
AM_CONDITIONAL([HAS_VERSION_SCRIPT], [test "$has_version_script" = "yes"])

AM_PROG_AR
LT_INIT

AM_GNU_GETTEXT_VERSION([0.18.2])
AM_GNU_GETTEXT([external])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug], [Enable extra debugging])])
AS_IF([test "x$enable_debug" = "xyes"],
	[CPPFLAGS="$CPPFLAGS -DDEBUG"],
	[CPPFLAGS="$CPPFLAGS -DNDEBUG"])

dnl Automatically increment the revision for every release.
LT_REVISION=$(echo "${PACKAGE_VERSION}" | tr -d .)
AC_SUBST(LT_REVISION)

AC_CONFIG_COMMANDS([include/attr],
	[dnl
	mkdir -p include/attr
	rm -f include/attr/*.h
	$as_ln_s "$ac_abs_top_srcdir"/include/*.h include/attr
	])
AC_CONFIG_FILES([
	libattr.pc
	Makefile
	po/Makefile.in
])
AC_OUTPUT
