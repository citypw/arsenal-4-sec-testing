#!/bin/bash

# Monkey-coder: Shawn the R0ck,<citypw@gmail.com>
# GPL'ed....

echo "=====================[ Audit for linkers on GNU/Linux ]===================="
echo "###########################################################################"

echo " This is free software, and you are welcome to redistribute it
 under the terms of the GNU General Public License.  See LICENSE file
 for details about using this software."  

echo "\nThis program was inspired by Tim Brown's paper about exploitable linker."

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

SETUID_PROGS=`find /bin /sbin /usr/bin /usr/sbin ./  -type f -perm -4000`

for i in $SETUID_PROGS; do
	objdump -x $i | grep -i path > /dev/null
	if [ $? -eq 0 ]; then
		echo -e "*- \e[91mPlease check this binary: $i"
	fi
done
