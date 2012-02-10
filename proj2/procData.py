#!/usr/bin/python2
# Processes data produced by p1c.c/p1d.c into a format appropriate for gnuplot
# Author: Sean Barag <sjb89@drexel.edu>

# read data
data = []
f = open("timing_data.txt", "r")
for line in f:
	data.append(float(line.strip()))
f.close()

# normalize data to time since first packet
init = data[0]
data = [ (val - init) for val in data ]


# floor data
mfloor = [int(val // 60) for val in data]
sfloor = [int(val // 1) for val in data]
ufloor = [int(val // 0.001) for val in data]
floored = [ mfloor, sfloor, ufloor ]

# bin data
vals = [ [],[],[] ]
counts = [ [],[],[] ]
for bin_i in range(3):
	for val in floored[bin_i]:
		if val in vals[bin_i]:
			counts[bin_i][ vals[bin_i].index(val) ] += 1
		else:
			vals[bin_i].append(val)
			counts[bin_i].append(1)

# print to file
f = []
f.append( open("binned_data_m.txt", "w") )
f.append( open("binned_data_s.txt", "w") )
f.append( open("binned_data_us.txt", "w") )

for i in range(len(vals)):
	for v,c in zip(vals[i], counts[i]):
		f[i].write( '{:d}{:s}{:d}{:s}'.format(v, '\t\t', c, '\n') )
		#print v, "\t\t", c

for i in range(len(f)):
	f[i].close()
