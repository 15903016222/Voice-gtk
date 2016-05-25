#!/bin/sh

version=`git lg | grep "tag:" | head -n 1 | cut -d":" -f2 | cut -d")" -f1 | cut -d"," -f1 | cut -d" " -f2`


echo ${version}

