#!/bin/bash


echo "=====================[ Audit for linkers on GNU/Linux ]===================="
echo "###########################################################################"

echo " Monkey-coder: Shawn the R0ck,<citypw@gmail.com>"
echo " GPL'ed...."

echo " This is free software, and you are welcome to redistribute it
 under the terms of the GNU General Public License.  See LICENSE file
 for details about using this software."  

echo "This program was inspired by Tim Brown's paper about exploitable linker."
echo "Do some security baseline checks..."

echo "###########################################################################"
echo -e "[+] \e[93mChecking LD_LIBRARY_PATH\e[0m"
echo "----------------------------------------------"

touch ./libc.so.6 && echo "hello world" & > /dev/null

if [ $? -eq 0 ]; then
	echo -e "*- LD_LIBRARY_PATH:\e[92m OK\e[0m"
else
	echo -e "*- LD_LIBRARY_PATH:\e[91m FAILED\e[0m, plz check..."
fi
echo "----------------------------------------------\n"

echo -e "[+] \e[93mChecking setuid() programs in /bin, /sbin, /usr/bin, /usr/sbin, $PWD\e[0m"
echo "----------------------------------------------"

SETUID_PROGS=`find / -type f -perm -4000`

echo $SETUID_PROGS

for i in $SETUID_PROGS; do
	objdump -x $i | grep -i path > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "*- \e[91mPlease check this binary: $i"
	fi
done

echo "======================[ System audit for GNU/Linux ]======================="
echo "###########################################################################"

echo "###########################################################################"
echo -e "[+] \e[93mChecking potential threats of Wildcards\e[0m"
echo "----------------------------------------------"

find /   -name "-*"

echo "###########################################################################"
echo -e "[+] \e[93mChecking world-writable permission files\e[0m"
echo "----------------------------------------------"

find / -path /proc -prune -o -perm -2 ! -type l -ls


echo "###########################################################################"
echo -e "[+] \e[93mChecking orphanage files without owner\e[0m"
echo "----------------------------------------------"

find / -path /proc -prune -o -nouser -o -nogroup


echo "###########################################################################"
echo -e "[+] \e[93mChecking live/usable users\e[0m"
echo "----------------------------------------------"

egrep -v '.*:\*|:\!' /etc/shadow | awk -F: '{print $1}'
grep -v ':x:' /etc/passwd
