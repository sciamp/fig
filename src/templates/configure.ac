AC_PREREQ(2.60)

AC_INIT([@@NAME@@],[@@VERSION@@])
AC_CONFIG_MACRO_DIRS([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools])
AC_SUBST(ACLOCAL_AMFLAGS, "-I build/autotools")

. build/autotools/configure/*.m4

dnl Fake gtkdocize into thinking GTK_DOC_CHECK is called from configure \
GTK_DOC_CHECK
