dnl $Id$
dnl config.m4 for extension concurrency

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl Make sure that the comment is aligned:
PHP_ARG_WITH(concurrency, for concurrency support,
[  --with-concurrency             Include concurrency support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(concurrency, whether to enable concurrency support,
dnl Make sure that the comment is aligned:
dnl [  --enable-concurrency           Enable concurrency support])

if test "$PHP_CONCURRENCY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-concurrency -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/concurrency.h"  # you most likely want to change this
  dnl if test -r $PHP_CONCURRENCY/$SEARCH_FOR; then # path given as parameter
  dnl   CONCURRENCY_DIR=$PHP_CONCURRENCY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for concurrency files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CONCURRENCY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CONCURRENCY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the concurrency distribution])
  dnl fi

  dnl # --with-concurrency -> add include path
  dnl PHP_ADD_INCLUDE($CONCURRENCY_DIR/include)

  dnl # --with-concurrency -> check for lib and symbol presence
  dnl LIBNAME=concurrency # you may want to change this
  dnl LIBSYMBOL=concurrency # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CONCURRENCY_DIR/lib, CONCURRENCY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CONCURRENCYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong concurrency lib version or lib not found])
  dnl ],[
  dnl   -L$CONCURRENCY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CONCURRENCY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(concurrency, concurrency.c, $ext_shared)
fi
