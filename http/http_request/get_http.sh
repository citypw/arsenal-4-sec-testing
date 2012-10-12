#!/bin/sh

if [ -z $1 ]; then
	echo -e "\n\tUsage: $0 <host> <URL> <PORT>"
	exit
fi

echo -e "GET $2 HTTP/1.0\n\n" | nc -vv $1 $3
