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
tdict = [{}, {}, {}]
for bin_i in range(3):
	for val in floored[bin_i]:
		if val in tdict[bin_i]:
			tdict[bin_i][val] += 1
		else:
			tdict[bin_i][val] = 1

# open tfiles
f = [] # array of file descriptors
f.append( open("binned_data_m.txt", "w") )
f.append( open("binned_data_s.txt", "w") )
f.append( open("binned_data_ms.txt", "w") )
#f.append( open("data/binned_data_ms.txt", "w") )
#f.append( open("data/binned_data_s.txt", "w") )
#f.append( open("data/binned_data_ms.txt", "w") )

# write tdata
for i in range(3):
	for v,c in tdict[i].items():
		f[i].write( '{:d}{:s}{:d}{:s}'.format(v, '\t\t', c, '\n') )

#close tfiles
for i in range(len(f)):
	f[i].close()

# ---------- size data ----------
# bin sdata
cdict = {}
for size in sdata:
	if size in cdict:
		cdict[size] += 1
	else:
		cdict[size] = 1

# calculate frequencies
pktcount = sum( cdict.values() )
for k,v in cdict.items():
	cdict[k] = float(v)/float(pktcount)

# write sdata
f = open("frequency_data.txt", "w")
#f = open("data/frequency_data.txt", "w")
for v,c in cdict.items():
	f.write( '{:d}{:s}{:f}{:s}'.format(v, '\t\t', c, '\n') )
f.close()
