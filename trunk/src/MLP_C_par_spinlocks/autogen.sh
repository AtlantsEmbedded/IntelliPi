#!/bin/sh
#! /bin/sh

aclocal \
&& automake --add-missing \
&& autoreconf --install
