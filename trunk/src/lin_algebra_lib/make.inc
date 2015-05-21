# -*- Makefile -*-
####################################################################
#  LAPACK make include file.                                       #
#  LAPACK, Version 3.2.1                                           #
#  June 2009		                                               #
####################################################################
#
# See the INSTALL/ directory for more examples.
#
SHELL = /bin/sh

# This is used to compile C libary
CC        = gcc
# if no wrapping of the blas library is needed, uncomment next line
#CC        = gcc -DNO_BLAS_WRAP
CFLAGS    = -O3 -I$(TOPDIR)/includes
LOADER    = gcc
LOADOPTS  =
NOOPT     = -O0 -I$(TOPDIR)/includes
F2CCFLAGS = $(CFLAGS)
#######################################################################

ARCH     = ar
ARCHFLAGS= cr
RANLIB   = ranlib

#
#  Names of generated libraries.
#
SMALLLAPACKLIB    = libsmalllapack.a
F2CLIB       = libf2c.a
