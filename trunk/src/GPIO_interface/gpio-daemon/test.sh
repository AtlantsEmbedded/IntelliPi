#!/bin/sh
# -----------------------------------------------------------
# Ronnie Brash (ron.brash@gmail.com)
# Atomsproducts.com || pacificsimplicity.ca
# -----------------------------------------------------------
# Simple test script for use by the GPIO daemon functionality

# Echo PID into file in /tmp
echo $$ > /tmp/test.pid

# Simple cleanup function to remove pid file
clean_up() {

	rm -f /tmp/test.pid
	exit $1
}

# SEE http://mywiki.wooledge.org/SignalTrap
trap 'clean_up;' HUP INT QUIT KILL

# Sleep script for X amount of seconds
sleep $1

# Sleep a bit more then send a packet to the GPIO daemon
# for the send event condition
if [ "$2" = "send" ]; then
	sleep $1
	echo  "sleep event"
fi

# Sleep a bit more then send a packet to the GPIO daemon
# for the error condition
if [ "$2" = "error" ]; then
        sleep $1
        echo  "error event"
fi

# Loop for awhile then respond OR do nothing :)
# Useful for the cancel/kill event
if [ "$3" = "loop" ]; then
	while :
	do
		sleep $1
		echo "loop"
	done

fi

