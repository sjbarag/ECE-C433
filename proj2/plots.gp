# gnuplot script for p1c and p1d

# global
set terminal png
unset key
set ylabel "Number of Packets"

# minutes
set title "Packet Timing by Minute"
set xlabel "Time (minutes)"
set output "by_minute.png"
plot "binned_data_m.txt" using 1:2 with impulses

# seconds
set title "Packet Timing by Second"
set xlabel "Time (seconds)"
set output "by_second.png"
plot "binned_data_s.txt" using 1:2 with impulses

# milliseconds
set title "Packet Timing by Millisecond"
set xlabel "Time (milliseconds)"
set output "by_millisecond.png"
plot "binned_data_ms.txt" using 1:2 with impulses

# CDF
set title "Packet Size Distribution"
set xlabel "Packet Size (Bytes)"
set ylabel "Frequency"
set output "cdf.png"
plot "frequency_data.txt" using 1:2 smooth cumulative
