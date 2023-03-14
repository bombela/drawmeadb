#!/bin/bash

# Bombela - 2008
# bombela@free.fr
# $Id: dist.sh 85 2008-07-25 22:24:00Z bombela $

Version="$(grep "MeriseAcide_Version" MeriseAcide.h | awk -F\" '{ print $2 }')"
Dir="MeriseAcide$Version"
Tar="MeriseAcide$Version.tar.bz2"

rm -rf "$Dir"
svn export dist "$Dir"
cp syntax.ma "$Dir/."
cp CHANGELOG "$Dir/."
cp COPYING "$Dir/."
tar cvjf "$Tar" "$Dir"
rm -rf "$Dir"
