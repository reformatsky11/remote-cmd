AC_INIT(remotecmd, 0.1)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

dnl Check for the presence of SSL libraries and headers

dnl Default to compiler & linker defaults for SSL files & libraries.
OPT_SSL=off
dnl Default to no CA bundle
ca="no"
AC_ARG_WITH(ssl,dnl
AC_HELP_STRING([--with-ssl=PATH],[Where to look for OpenSSL, PATH points to the SSL installation (default: /usr/local/ssl)]),
OPT_SSL=$withval)

if test X"$OPT_SSL" != Xno; then
  dnl backup the pre-ssl variables
  CLEANLDFLAGS="$LDFLAGS"
  CLEANCPPFLAGS="$CPPFLAGS"
  CLEANLIBS="$LIBS"

  case "$OPT_SSL" in
  yes)
    dnl --with-ssl (without path) used
    if test x$cross_compiling != xyes; then
      dnl only do pkg-config magic when not cross-compiling
      PKGTEST="yes"
    fi
    PREFIX_OPENSSL=/usr/local/ssl
    LIB_OPENSSL="$PREFIX_OPENSSL/lib$libsuff"
    ;;
  off)
    dnl no --with-ssl option given, just check default places
    if test x$cross_compiling != xyes; then
      dnl only do pkg-config magic when not cross-compiling
      PKGTEST="yes"
    fi
    PREFIX_OPENSSL=
    ;;
  *)
    dnl check the given --with-ssl spot
    PKGTEST="no"
    PREFIX_OPENSSL=$OPT_SSL
    LIB_OPENSSL="$PREFIX_OPENSSL/lib$libsuff"
    LDFLAGS="$LDFLAGS -L$LIB_OPENSSL"
    CPPFLAGS="$CPPFLAGS -I$PREFIX_OPENSSL/include/openssl -I$PREFIX_OPENSSL/include"
    ;;
  esac

  if test "$PKGTEST" = "yes"; then

    dnl Detect the pkg-config tool, as it may have extra info about the
    dnl openssl installation we can use. I *believe* this is what we are
    dnl expected to do on really recent Redhat Linux hosts.

    AC_PATH_PROG( PKGCONFIG, pkg-config, no, $PATH:/usr/bin:/usr/local/bin)
    if test "$PKGCONFIG" != "no" ; then
      AC_MSG_CHECKING([OpenSSL options with pkg-config])

      $PKGCONFIG --exists openssl
      SSL_EXISTS=$?

      if test "$SSL_EXISTS" -eq "0"; then
        SSL_LIBS=`$PKGCONFIG --libs-only-l openssl 2>/dev/null`
        SSL_LDFLAGS=`$PKGCONFIG --libs-only-L openssl 2>/dev/null`
        SSL_CPPFLAGS=`$PKGCONFIG --cflags-only-I openssl 2>/dev/null`

        LIB_OPENSSL=`echo $SSL_LDFLAGS | sed -e 's/-L//g'`

        dnl use the values pkg-config reported
        LIBS="$LIBS $SSL_LIBS"
        CPPFLAGS="$CPPFLAGS $SSL_CPPFLAGS"
        LDFLAGS="$LDFLAGS $SSL_LDFLAGS"
        AC_MSG_RESULT([found])
      else
        AC_MSG_RESULT([no])
      fi
    fi
  fi

  dnl This is for Msys/Mingw
  case $host in
    *-*-cygwin*)
      dnl Under Cygwin this is extraneous and causes an unnecessary -lgdi32
      dnl to be added to LIBS and recorded in the .la file.
      ;;
    *)
      AC_MSG_CHECKING([for gdi32])
      my_ac_save_LIBS=$LIBS
      LIBS="-lgdi32 $LIBS"
      AC_TRY_LINK([#include <windef.h>
                   #include <wingdi.h>],
                   [GdiFlush();],
                   [ dnl worked!
                   AC_MSG_RESULT([yes])],
                   [ dnl failed, restore LIBS
                   LIBS=$my_ac_save_LIBS
                   AC_MSG_RESULT(no)]
                  )
      ;;
  esac

  AC_CHECK_LIB(crypto, CRYPTO_lock,[
     HAVECRYPTO="yes"
     LIBS="-lcrypto $LIBS"
     ],[
     LDFLAGS="$CLEANLDFLAGS -L$LIB_OPENSSL"
     CPPFLAGS="$CLEANCPPFLAGS -I$PREFIX_OPENSSL/include/openssl -I$PREFIX_OPENSSL/include"
     AC_CHECK_LIB(crypto, CRYPTO_add_lock,[
       HAVECRYPTO="yes"
       LIBS="-lcrypto $LIBS"], [
       LDFLAGS="$CLEANLDFLAGS"
       CPPFLAGS="$CLEANCPPFLAGS"
       LIBS="$CLEANLIBS"
       ])
    ])


  if test X"$HAVECRYPTO" = X"yes"; then
    dnl This is only reasonable to do if crypto actually is there: check for
    dnl SSL libs NOTE: it is important to do this AFTER the crypto lib

    AC_CHECK_LIB(ssl, SSL_connect)

    if test "$ac_cv_lib_ssl_SSL_connect" != yes; then
        dnl we didn't find the SSL lib, try the RSAglue/rsaref stuff
        AC_MSG_CHECKING(for ssl with RSAglue/rsaref libs in use);
        OLIBS=$LIBS
        LIBS="$LIBS -lRSAglue -lrsaref"
        AC_CHECK_LIB(ssl, SSL_connect)
        if test "$ac_cv_lib_ssl_SSL_connect" != yes; then
            dnl still no SSL_connect
            AC_MSG_RESULT(no)
            LIBS=$OLIBS
        else
            AC_MSG_RESULT(yes)
        fi

    else

      dnl Have the libraries--check for SSLeay/OpenSSL headers
      AC_CHECK_HEADERS(openssl/x509.h openssl/rsa.h openssl/crypto.h \
                       openssl/pem.h openssl/ssl.h openssl/err.h,
        ssl_msg="enabled (OpenSSL)"
        OPENSSL_ENABLED=1
        AC_DEFINE(USE_OPENSSL, 1, [if OpenSSL is in use]))

      if test $ac_cv_header_openssl_x509_h = no; then
        dnl we don't use the "action" part of the AC_CHECK_HEADERS macro
        dnl since 'err.h' might in fact find a krb4 header with the same
        dnl name
        AC_CHECK_HEADERS(x509.h rsa.h crypto.h pem.h ssl.h err.h)

        if test $ac_cv_header_x509_h = yes &&
           test $ac_cv_header_crypto_h = yes &&
           test $ac_cv_header_ssl_h = yes; then
          dnl three matches
          ssl_msg="enabled (OpenSSL)"
          OPENSSL_ENABLED=1
        fi
      fi
    fi

    if test X"$OPENSSL_ENABLED" = X"1"; then
       AC_DEFINE(USE_SSLEAY, 1, [if SSL is enabled])

       dnl is there a pkcs12.h header present?
       AC_CHECK_HEADERS(openssl/pkcs12.h)
    else
       LIBS="$CLEANLIBS"
    fi
    dnl USE_SSLEAY is the historical name for what configure calls
    dnl OPENSSL_ENABLED; the names should really be unified
    USE_SSLEAY="$OPENSSL_ENABLED"
    AC_SUBST(USE_SSLEAY)

    if test X"$OPT_SSL" != Xoff &&
       test "$OPENSSL_ENABLED" != "1"; then
      AC_MSG_ERROR([OpenSSL libs and/or directories were not found where specified!])
    fi
  fi

  if test X"$OPENSSL_ENABLED" = X"1"; then
    dnl If the ENGINE library seems to be around, check for the OpenSSL engine
    dnl stuff, it is kind of "separated" from the main SSL check
    AC_CHECK_FUNC(ENGINE_init,
              [
                AC_CHECK_HEADERS(openssl/engine.h)
                AC_CHECK_FUNCS( ENGINE_load_builtin_engines )
              ])

    dnl these can only exist if openssl exists
    dnl yassl doesn't have SSL_get_shutdown

    AC_CHECK_FUNCS( RAND_status \
                    RAND_screen \
                    RAND_egd \
                    CRYPTO_cleanup_all_ex_data \
                    SSL_get_shutdown )

    dnl Make an attempt to detect if this is actually yassl's headers and
    dnl OpenSSL emulation layer. We still leave everything else believing
    dnl and acting like OpenSSL.

    AC_MSG_CHECKING([for yaSSL using OpenSSL compatibility mode])
    AC_COMPILE_IFELSE([
      AC_LANG_PROGRAM([[
#include <openssl/ssl.h>
      ]],[[
#if defined(YASSL_VERSION) && defined(OPENSSL_VERSION_NUMBER)
        int dummy = SSL_ERROR_NONE;
#else
        Not the yaSSL OpenSSL compatibility header.
#endif
      ]])
    ],[
      AC_MSG_RESULT([yes])
      AC_DEFINE_UNQUOTED(USE_YASSLEMUL, 1,
        [Define to 1 if using yaSSL in OpenSSL compatibility mode.])
      ssl_msg="enabled (OpenSSL emulation by yaSSL)"
    ],[
      AC_MSG_RESULT([no])
    ])
  fi

  if test "$OPENSSL_ENABLED" = "1"; then
    if test -n "$LIB_OPENSSL"; then
       dnl when the ssl shared libs were found in a path that the run-time
       dnl linker doesn't search through, we need to add it to LD_LIBRARY_PATH
       dnl to prevent further configure tests to fail due to this

       LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$LIB_OPENSSL"
       export LD_LIBRARY_PATH
       AC_MSG_NOTICE([Added $LIB_OPENSSL to LD_LIBRARY_PATH])
    fi
  fi

fi

dnl **********************************************************************
dnl Check for the presence of ZLIB libraries and headers
dnl **********************************************************************

dnl Check for & handle argument to --with-zlib.

_cppflags=$CPPFLAGS
_ldflags=$LDFLAGS
AC_ARG_WITH(zlib,
AC_HELP_STRING([--with-zlib=PATH],[search for zlib in PATH]),
[OPT_ZLIB="$withval"])

if test "$OPT_ZLIB" = "no" ; then
    AC_MSG_WARN([zlib disabled])
else
  if test "$OPT_ZLIB" = "yes" ; then
     OPT_ZLIB=""
  fi

  if test -z "$OPT_ZLIB" ; then
    dnl check for the lib first without setting any new path, since many
    dnl people have it in the default path

    AC_CHECK_LIB(z, inflateEnd,
                   dnl libz found, set the variable
                   [HAVE_LIBZ="1"],
                   dnl if no lib found, try /usr/local
                   [OPT_ZLIB="/usr/local"])

  fi

  dnl Add a nonempty path to the compiler flags
  if test -n "$OPT_ZLIB"; then
     CPPFLAGS="$CPPFLAGS -I$OPT_ZLIB/include"
     LDFLAGS="$LDFLAGS -L$OPT_ZLIB/lib$libsuff"
  fi

  AC_CHECK_HEADER(zlib.h,
    [
    dnl zlib.h was found
    HAVE_ZLIB_H="1"
    dnl if the lib wasn't found already, try again with the new paths
    if test "$HAVE_LIBZ" != "1"; then
      AC_CHECK_LIB(z, gzread,
                   [
                   dnl the lib was found!
                   HAVE_LIBZ="1"
                   ],
                   [ CPPFLAGS=$_cppflags
                   LDFLAGS=$_ldflags])
    fi
    ],
    [
      dnl zlib.h was not found, restore the flags
      CPPFLAGS=$_cppflags
      LDFLAGS=$_ldflags]
    )

  if test "$HAVE_LIBZ" = "1" && test "$HAVE_ZLIB_H" != "1"
  then
    AC_MSG_WARN([configure found only the libz lib, not the header file!])
    HAVE_LIBZ=""
  elif test "$HAVE_LIBZ" != "1" && test "$HAVE_ZLIB_H" = "1"
  then
    AC_MSG_WARN([configure found only the libz header file, not the lib!])
  elif test "$HAVE_LIBZ" = "1" && test "$HAVE_ZLIB_H" = "1"
  then
    dnl both header and lib were found!
    AC_SUBST(HAVE_LIBZ)
    AC_DEFINE(HAVE_ZLIB_H, 1, [if you have the zlib.h header file])
    AC_DEFINE(HAVE_LIBZ, 1, [if zlib is available])

    LIBS="$LIBS -lz"
    LIBS="$LIBS -lz"

    dnl replace 'HAVE_LIBZ' in the automake makefile.ams
    AMFIXLIB="1"
    AC_MSG_NOTICE([found both libz and libz.h header])
    zlib_msg="enabled"
  fi
fi

dnl set variable for use in automakefile(s)
AM_CONDITIONAL(HAVE_LIBZ, test x"$AMFIXLIB" = x1)

dnl **********************************************************************
dnl Check for the presence of LIBSSH2 libraries and headers
dnl **********************************************************************

dnl Default to compiler & linker defaults for LIBSSH2 files & libraries.
OPT_LIBSSH2=off
AC_ARG_WITH(libssh2,dnl
AC_HELP_STRING([--with-libssh2=PATH],[Where to look for libssh2, PATH points to the LIBSSH2 installation (default: /usr/local/lib)]),
OPT_LIBSSH2=$withval)

if test X"$OPT_LIBSSH2" != Xno; then
  dnl backup the pre-libssh2 variables
  CLEANLDFLAGS="$LDFLAGS"
  CLEANCPPFLAGS="$CPPFLAGS"
  CLEANLIBS="$LIBS"

  case "$OPT_LIBSSH2" in
  yes)
    dnl --with-libssh2 (without path) used
    PREFIX_LIBSSH2=/usr/local/lib
    LIB_LIBSSH2="$PREFIX_LIBSSH2$libsuff"
    ;;
  off)
    dnl no --with-libssh2 option given, just check default places
    PREFIX_LIBSSH2=
    ;;
  *)
    dnl use the given --with-libssh2 spot
    PREFIX_LIBSSH2=$OPT_LIBSSH2
    LIB_LIBSSH2="$PREFIX_LIBSSH2/lib$libsuff"
    LDFLAGS="$LDFLAGS -L$LIB_LIBSSH2"
    CPPFLAGS="$CPPFLAGS -I$PREFIX_LIBSSH2/include"
    ;;
  esac

  AC_CHECK_LIB(ssh2, libssh2_channel_open_ex)

  AC_CHECK_HEADERS(libssh2.h,
    ssh_msg="enabled (libSSH2)"
    LIBSSH2_ENABLED=1
    AC_DEFINE(USE_LIBSSH2, 1, [if libSSH2 is in use])
    AC_SUBST(USE_LIBSSH2, [1])
  )

  if test X"$OPT_LIBSSH2" != Xoff &&
     test "$LIBSSH2_ENABLED" != "1"; then
    AC_MSG_ERROR([libSSH2 libs and/or directories were not found where specified!])
  fi

  if test "$LIBSSH2_ENABLED" = "1"; then
    if test -n "$LIB_LIBSSH2"; then
       dnl when the libssh2 shared libs were found in a path that the run-time
       dnl linker doesn't search through, we need to add it to LD_LIBRARY_PATH
       dnl to prevent further configure tests to fail due to this

       LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$LIB_LIBSSH2"
       export LD_LIBRARY_PATH
       AC_MSG_NOTICE([Added $LIB_LIBSSH2 to LD_LIBRARY_PATH])
    fi
  fi
fi

AM_PROG_LIBTOOL

AC_OUTPUT([
Makefile
src/Makefile

])

