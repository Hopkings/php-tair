dnl $Id$
dnl config.m4 for extension tair

SEARCH_PATH="/usr/local/tblib /usr/local /usr"
SEARCH_FOR="/include/tbsys/tbsys.h"
if test -r $TBLIB_ROOT$SEARCH_FOR; then
	AC_MSG_RESULT([found])
else
    AC_MSG_CHECKING([for tblib_common files in default path])
    for i in $SEARCH_PATH ; do
       if test -r $i$SEARCH_FOR; then
         TBLIB_ROOT=$i
         AC_MSG_RESULT(found in $i)
       fi
    done
fi

if test -r $TBLIB_ROOT$SEARCH_FOR; then
	INCLUDES+=" $TBLIB_ROOT/include "
	LDFLAGS+=" -L$TBLIB_ROOT/lib -ltbnet -ltbsys "
else
	AC_MSG_RESULT([$TBLIB_ROOT not found])
	AC_MSG_ERROR([Please define \$TBLIB_ROOT])
fi

PHP_ARG_WITH(tair, for tair support,
[  --with-tair             Specal tair source dir])

if test "$PHP_TAIR" != "no"; then
  SEARCH_PATH="/usr/local/tair /usr/local /usr"
  SEARCH_FOR="/include/tair_client_api.hpp"
  if test -r $PHP_TAIR/$SEARCH_FOR; then
    TAIR_DIR=$PHP_TAIR
   else
     AC_MSG_CHECKING([for tair files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR; then
         TAIR_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
   fi
  
  if test -z "$TAIR_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the tair distribution])
  fi

  # --with-tair -> add include path
  PHP_ADD_INCLUDE($TAIR_DIR/include)

  # --with-tair -> check for lib and symbol presence
  LIBNAME="tairclientapi_c" # you may want to change this
  LIBSYMBOL=tair_init # you most likely want to change this 

  # lib
  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TAIR_DIR/lib, TAIR_SHARED_LIBADD)
    AC_DEFINE(HAVE_TAIRLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong tair lib version or lib not found])
  ],[
    -L$TAIR_DIR/lib -lm
  ])
  PHP_SUBST(TAIR_SHARED_LIBADD)


dnl PHP_REQUIRE_CXX()
  PHP_NEW_EXTENSION(tair, tair.c,$ext_shared)
fi
