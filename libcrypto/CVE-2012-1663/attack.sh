#!/bin/sh

COUNTER=0
while [ $COUNTER -lt 100 ]; do
	echo trying dos... $COUNT
	./client &
	let COUNTER=COUNTER+1
done

