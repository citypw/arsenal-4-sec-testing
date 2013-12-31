#!/usr/bin/env python

import sys

def print_usage():
	print "Usage: %s START_ADDR\n" % (sys.argv[0])

if __name__ == "__main__":
	try:
		START = int(sys.argv[1], 16)
	except:
		print_usage()
		quit()

	f = open('map-003.bin', 'r')
	read_data = f.read()
	f.close

	loc = int(read_data.find("/bin/sh"))
	loc += START
	print "RESULT: START(%x) + targeted_location = 0x%x" % (START, loc)
