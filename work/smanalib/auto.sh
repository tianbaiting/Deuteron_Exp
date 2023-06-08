#!/bin/sh
#
#srcdir=`dirname $0`
#test -z "$srcdir" && srcdir=.
#
#(cd $srcdir; aclocal;\
#libtoolize --force; automake -a --add-missing; autoconf)
#
#$srcdir/configure "$@"

autoheader
aclocal
libtoolize --force
automake -a --add-missing --copy
autoconf

source configure --prefix=$PWD
