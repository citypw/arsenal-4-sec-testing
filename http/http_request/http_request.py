#!/usr/bin/env python

"""
This program is also GPL'ed
"""

import sys
import socket
import time
import re

def print_usage():
        print "Usage: %s TARGET_SERVER TARGET_PORT \"HTTP REQUEST\"\n" % (sys.argv[0])

try:
#socket.gethostbyname("www.google.com") ?
	HOST = sys.argv[1]
	PORT = int(sys.argv[2])

except:
	print_usage()
	quit()

REQ = sys.argv[3].replace("\\r\\n", "\r\n")

#REQ = "HEAD / HTTP/1.1\r\nHost: " + sys.argv[1] + "\r\n\r\n"
sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockfd.connect((HOST, PORT))

print "send:\n%s" % REQ

#print repr(REQ)
sockfd.send(REQ)

buf = sockfd.recv(512)
print "Response:\n%s" % buf

sockfd.close()
