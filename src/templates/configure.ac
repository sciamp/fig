AC_PREREQ(2.60)

# Initialize autoconf and things that go with it.
. build/autotools/configure.d/*.pre-ac
AC_INIT([name],[version])
AC_CONFIG_MACRO_DIRS([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools])
AC_SUBST(ACLOCAL_AMFLAGS, "-I build/autotools")
AC_CONFIG_HEADERS([config.h])
. build/autotools/configure.d/*.post-ac

# Initlize libtool and things that go with it.
. build/autotools/configure.d/*.pre-lt
. build/autotools/configure.d/setup_libtool.m4
. build/autotools/configure.d/*.post-lt

# Initialize automake and things that go with it.
# We would put AM initialization in a file but autoreconf requires it.
. build/autotools/configure.d/*.pre-am
AM_INIT_AUTOMAKE([foreign subdir-objects tar-ustar])
AM_MAINTAINER_MODE([enable])
. build/autotools/configure.d/*.post-am

# If we are using gtk-doc, enable it.
# We would put this in a file but gtkdocize requires it.
m4_ifdef([GTK_DOC_CHECK],[
GTK_DOC_CHECK
])



