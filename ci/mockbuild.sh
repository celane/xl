#!/usr/bin/bash

NAME=xl
VERSION=$1

dnf install -y epel-release
dnf install -y mock

tar -xf ${NAME}-${VERSION}.tar.gz ${NAME}-${VERSION}/${NAME}.spec

RELEASE=$(grep 'Release: ' ${NAME}-${VERSION}/${NAME}.spec | cut -d ':' -f2 | awk -F'%' '{print $1}' | tr -d ' ')
echo RELEASE=${RELEASE} >> $GITHUB_ENV

mock -r rocky+epel-8-x86_64 --rebuild --spec=${NAME}-${VERSION}/${NAME}.spec --sources=.
mv /var/lib/mock/rocky+epel-8-x86_64/result/${NAME}-${VERSION}-${RELEASE}.el8.x86_64.rpm .
mv /var/lib/mock/rocky+epel-8-x86_64/result/${NAME}-${VERSION}-${RELEASE}.el8.src.rpm .

mock -r centos+epel-7-x86_64 --rebuild --spec=${NAME}-${VERSION}/${NAME}.spec --sources=.
mv /var/lib/mock/centos+epel-7-x86_64/result/${NAME}-${VERSION}-${RELEASE}.el7.x86_64.rpm .
mv /var/lib/mock/centos+epel-7-x86_64/result/${NAME}-${VERSION}-${RELEASE}.el7.src.rpm .

mock -r opensuse-leap-15.3-x86_64 --rebuild --spec=${NAME}-${VERSION}/${NAME}.spec --sources=.
mv /var/lib/mock/opensuse-leap-15.3-x86_64/result/${NAME}-${VERSION}-${RELEASE}.suse.lp153.x86_64.rpm .
mv /var/lib/mock/opensuse-leap-15.3-x86_64/result/${NAME}-${VERSION}-${RELEASE}.suse.lp153.src.rpm .
