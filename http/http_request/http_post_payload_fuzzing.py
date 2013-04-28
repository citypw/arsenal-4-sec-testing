#!/usr/bin/env python

"""
This shitty program is only doing the fuzzing job. Generate random byte as HTTP POST request payload.

MOnkey-coder: Shawn the R0ck
This program is also GPL'ed
"""

import sys
import socket
import time
import re
from random import Random
import string
import random

def random_str(randomlength=8):
        str = ''
        chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789~!@#$%^&*()_+-=\][\';/.,|}{":?><'
        length = len(chars) - 1
        random = Random()
        for i in range(randomlength):
                str+=chars[random.randint(0, length)]
        return str

def print_usage():
        print "Usage: %s TARGET_SERVER TARGET_PORT \"HTTP REQUEST\"\n" % (sys.argv[0])

try:
#socket.gethostbyname("www.google.com") ?
	HOST = sys.argv[1]
	PORT = int(sys.argv[2])

except:
	print_usage()
	quit()


while 1:
	rnd = random.randint(0, 256)
	REQ = "POST http://192.168.0.20 HTTP/1.1\r\nHost: 192.168.0.20\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n%s" % ( rnd, random_str(random.randint(0, 256)))
	sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sockfd.connect((HOST, PORT))

	print "send:\n%s" % REQ

#print repr(REQ)
	sockfd.send(REQ)

	buf = sockfd.recv(512)
	print "Response:\n%s" % buf

	sockfd.close()
