#!/usr/bin/python2
# Processes Ethernet data

f = open("data/eth_data.txt", "r")

# skip header lines
[f.readline() for i in range(3) ]

data = []

for line in f:
	data.append(line.split()[2])

eth_types = {'Xerox PUP':0, 'Sprite':0, 'IPv4':0, 'ARP':0, 'Reverse ARP':0,
'AppleTalk ARP':0, 'IEEE 802.1Q VLAN tagging':0, 'IPX':0, 'IPv6':0,
'Loopback':0, 'Unknown':0}

for t in data:
	eth_types[t] += 1

print eth_types
