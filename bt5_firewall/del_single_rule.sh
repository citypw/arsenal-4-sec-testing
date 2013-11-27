#!/bin/sh

iptables -D PSAD_BLOCK_INPUT -s $1 -j DROP
