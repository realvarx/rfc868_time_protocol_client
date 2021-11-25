# Time protocol client
This program allows the user to connect to a TIME protocol server and get the current time.

Examples

usage:    ./atdate [-s <serverhost>] [-p <port>] [-m <cu\ct\s>] [-d]

- TCP CLIENT MODE

./atdate -s 192.100.100.103 -p 6665 -m ct -d 1

- UDP CLIENT MODE

./atdate -s 192.100.100.103 -p 6665 -m cu -d 1
./atdate -s time-a.timefreq.bldrdoc.gov -p 37 -m cu -d 0

- TCP/UDP CONCURRENT SERVER MODE

./atdate -p 6665 -m s -d 1
