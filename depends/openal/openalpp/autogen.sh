#!/bin/sh
aclocal
libtoolize --force
automake --foreign --add-missing
autoconf
./configure --disable-libtool-lock  $@
