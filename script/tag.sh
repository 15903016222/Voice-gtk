#!/bin/sh

MAJOR=1
MINOR=0
MICRO=0

version=1.0.0

for tag in `git tag`; do
	tmp=`echo ${tag} | awk -F"." '{print $1}'`
	if [ "${tmp}" -gt "${MAJOR}" ]; then
		MAJOR=${tmp}
	fi

	tmp=`echo ${tag} | awk -F"." '{print $2}'`
	if [ "${tmp}" -gt "${MINOR}" ]; then
		MINOR=${tmp}
	fi
	
	tmp=`echo ${tag} | awk -F"." '{print $3}'`
	if [ "${tmp}" -gt "${MICRO}" ]; then
		MICRO=${tmp}
	fi
done

echo ${MAJOR}.${MINOR}.${MICRO}
