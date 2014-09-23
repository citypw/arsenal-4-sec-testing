# By James Moore 

#!/bin/sh
 
#set -x
 
EMAIL=someone@example.com
OLDHOME=${HOME}
cd ~/lynis
 
. /etc/apache2/envvars
./lynis -c -Q -q --profile /etc/lynis/default.prf
 
HOME=${OLDHOME}
 
# Check to ensure we're still at 100%
grep -q 'Hardening index : \[100\]' /var/log/lynis.log
 
if [ $? != 0 ]; then
./lynis --check-update --no-colors | mutt -s 'lynis report' $EMAIL -a /var/log/lynis.log
fi
 
# notify if there's an update available
./lynis --check-update > /tmp/lynis.check
if [ $? != 0 ]; then
./lynis --check-update --no-colors | mutt -s 'lynis update available' $EMAIL
fi
rm /tmp/lynis.check
 
# remove the turd made by mutt
if [ -f ~/sent ]; then
rm ~/sent
fi
 
# End
