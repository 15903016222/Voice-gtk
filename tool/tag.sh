#!/bin/sh

MAJOR=1
MINOR=0
MICRO=0

version=1.0.0

for tag in `git tag`; do
	tmp=`echo ${tag} | awk -F"." '{print $1}'`
	if [ "${tmp}" -lt "${MAJOR}" ]; then
		continue
	fi
	MAJOR=${tmp}

	tmp=`echo ${tag} | awk -F"." '{print $2}'`
	if [ "${tmp}" -lt "${MINOR}" ]; then
		continue
	fi
	MINOR=${tmp}
	
	tmp=`echo ${tag} | awk -F"." '{print $3}'`
	if [ "${tmp}" -gt "${MICRO}" ]; then
		continue
	fi
	MICRO=${tmp}
done

echo ${MAJOR}.${MINOR}.${MICRO}

