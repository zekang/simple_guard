dnl $Id$
dnl config.m4 for extension simple_guard

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

 PHP_ARG_WITH(simple_guard, for simple_guard support,
 Make sure that the comment is aligned:
 [  --with-simple_guard             Include simple_guard support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(simple_guard, whether to enable simple_guard support,
dnl Make sure that the comment is aligned:
dnl [  --enable-simple_guard           Enable simple_guard support])

if test "$PHP_SIMPLE_GUARD" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-simple_guard -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/simple_guard.h"  # you most likely want to change this
  dnl if test -r $PHP_SIMPLE_GUARD/$SEARCH_FOR; then # path given as parameter
  dnl   SIMPLE_GUARD_DIR=$PHP_SIMPLE_GUARD
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for simple_guard files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SIMPLE_GUARD_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SIMPLE_GUARD_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the simple_guard distribution])
  dnl fi

  dnl # --with-simple_guard -> add include path
  dnl PHP_ADD_INCLUDE($SIMPLE_GUARD_DIR/include)

  dnl # --with-simple_guard -> check for lib and symbol presence
  dnl LIBNAME=simple_guard # you may want to change this
  dnl LIBSYMBOL=simple_guard # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIMPLE_GUARD_DIR/$PHP_LIBDIR, SIMPLE_GUARD_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIMPLE_GUARDLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong simple_guard lib version or lib not found])
  dnl ],[
  dnl   -L$SIMPLE_GUARD_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SIMPLE_GUARD_SHARED_LIBADD)

  PHP_NEW_EXTENSION(simple_guard, simple_guard.c, $ext_shared)
fi
