#! /bin/bash -x
# perform a demo installation of the perl bindings from scratch
# mvidner@suse.cz, 2003-11-20
# $Id$
set -o errexit
# these can be overriden when invoking the script
: ${DIR:=$HOME/yperl}
: ${PFX:=$DIR/prefix}

mkdir -p $DIR
cd $DIR
cvs -d :pserver:$USER@yast2-cvs.suse.de:/suse/yast2/cvs      co source/core
cvs -d :pserver:$USER@yast2-cvs.suse.de:/real-home/CVS/YaST2 co -r language_binding_branch source/perl-bindings
# devtools are needed because of autodocs :-/
cvs -d :pserver:$USER@yast2-cvs.suse.de:/real-home/CVS/YaST2 co source/devtools

cd $DIR/source/devtools
make -f Makefile.cvs
make install prefix=$PFX

cd $DIR/source/core
make -f Makefile.cvs all PREFIX=/usr
CXXFLAGS="-O0 -g3 -I/usr/include/YaST2" ./configure --prefix=$PFX
make
# ycp.pm fails, ok
make install || true

cd $DIR/source/perl-bindings
make -f Makefile.cvs all
# the -I is for liby2util
CXXFLAGS="-O0 -g3 -I/usr/include/YaST2" ./configure --prefix=$PFX
make all install -C src

# set path to modules so that imported.pm is found
moddir=`cd ../perl-bindings/doc/examples/; pwd`
install -d $PFX/share/YaST2
ln -snf $moddir $PFX/share/YaST2/modules
# compile modules
make -C doc

# run demo
cd $DIR/source/perl-bindings/doc/examples
../../../core/base/src/y2base -l - import.ycp testsuite
