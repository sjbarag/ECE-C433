#!/usr/bin/python2
# Processes data produced by p1c.c/p1d.c into a format appropriate for gnuplot
# Author: Sean Barag <sjb89@drexel.edu>

# read data
tdata = []
sdata = []
f = open("data/timing_data.txt", "r")
for line in f:
	tdata.append(float(line.split()[0]))
	sdata.append(int(line.split()[1]))
f.close()

# ---------- timing data ----------

# normalize tdata to time since first packet
init = tdata[0]
tdata = [ (val - init) for val in tdata ]

# floor tdata
mfloor = [int(val // 60) for val in tdata]
sfloor = [int(val // 1) for val in tdata]
ufloor = [int(val // 0.001) for val in tdata]
floored = [ mfloor, sfloor, ufloor ]

# bin tdata
tvals = [ [],[],[] ]
tcounts = [ [],[],[] ]
for bin_i in range(3):
	for val in floored[bin_i]:
		if val in tvals[bin_i]:
			tcounts[bin_i][ tvals[bin_i].index(val) ] += 1
		else:
			tvals[bin_i].append(val)
			tcounts[bin_i].append(1)

# open tfiles
f = [] # array of file descriptors
f.append( open("data/binned_data_m.txt", "w") )
f.append( open("data/binned_data_s.txt", "w") )
f.append( open("data/binned_data_ms.txt", "w") )

# write tdata
for i in range(len(tvals)):
	for v,c in zip(tvals[i], tcounts[i]):
		f[i].write( '{:d}{:s}{:d}{:s}'.format(v, '\t\t', c, '\n') )

#close tfiles
for i in range(len(f)):
	f[i].close()

# ---------- size data ----------
# bin sdata
scounts = [ [], [] ]
for size in sdata:
	if size in scounts[0]:
		scounts[1][ scounts[0].index(size) ] += 1
	else:
		scounts[0].append(size)
		scounts[1].append(1)

# calculate frequencies
pktcount = len(sdata)
scounts[1] = [ float(c)/float(pktcount) for c in scounts[1] ]

# write sdata
f = open("data/frequency_data.txt", "w")
for v,c in zip(scounts[0], scounts[1]):
	f.write( '{:d}{:s}{:f}{:s}'.format(v, '\t\t', c, '\n') )
f.close()
