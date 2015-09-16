AC_INIT([MLP_C_par_spinlocks], [1.0], [bug-report@address])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC
AM_PROG_CC_C_O
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
MLP_CFLAGS="-O2 -Iinclude"

AC_CHECK_LIB(pthread, pthread_spin_trylock)
AC_CHECK_LIB(rt,pthread_attr_setdetachstate)
AC_CHECK_LIB(m,exp)

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME

# Checks for typedefs and compiler characteristics.
AC_C_BIGENDIAN
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

AC_CHECK_HEADERS(unistd.h fcntl.h strings.h pthread.h)
AC_MSG_CHECKING(for usleep)
AH_TEMPLATE(HAVE_USLEEP)
AC_EGREP_HEADER(usleep, unistd.h, is_usleep=yes, is_usleep=no)
if test $is_usleep = yes; then
  AC_DEFINE(HAVE_USLEEP)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

# Set localized define flags
# Enable verbosity Define
AC_ARG_ENABLE([verbosity],[--enable-verbosity    Enable message verbosity])

if test "x$enable_verbosity" = xyes; then
	MLP_CFLAGS+=" -DVERBOSITY"
fi

AC_ARG_ENABLE([scheduler],[--enable-scheduler    Enable thread scheduling])

if test "x$enable_scheduler"  = xyes; then
  MLP_CFLAGS+=" -DSCHEDULER_POLICY"
fi

AC_SUBST(MLP_CFLAGS)
AC_OUTPUT
