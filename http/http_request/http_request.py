#!/usr/bin/env python

"""
This program is also GPL'ed
"""

import sys
import socket
import time
import re

#socket.gethostbyname("www.google.com") ?
HOST = sys.argv[1]
REQ = sys.argv[2].replace("\\r\\n", "\r\n")
PORT = 80

#REQ = "HEAD / HTTP/1.1\r\nHost: " + sys.argv[1] + "\r\n\r\n"
sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockfd.connect((HOST, PORT))

print "send:\n%s" % REQ

#print repr(REQ)
sockfd.send(REQ)

buf = sockfd.recv(512)
print "Response:\n%s" % buf

sockfd.close()
