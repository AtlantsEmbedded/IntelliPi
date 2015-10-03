#!/bin/sh
# ===============================================================
# @file usb-disk-test.sh
# @author Ron Brash (ron.brash@gmail.com) | Atlants Embedded 2015
# @brief USB storage detector script
# ===============================================================

DRIVE=""
MOUNT_PT=""
DATA_DIR="/tmp/"

test_for_drives() {
	REMOVABLE_DRIVES=""
	for _device in /sys/block/*/device; do
	    if echo $(readlink -f $_device)|egrep -q "usb"; then
	        _disk=`echo $_device | cut -f4 -d/`
	        REMOVABLE_DRIVES="$REMOVABLE_DRIVES $_disk"
	    fi
	done
	if [ "$REMOVABLE_DRIVES" = "" ] 
	then
		echo "No drives found"
	else
		echo $REMOVABLE_DRIVES
		DRIVE=$REMOVABLE_DRIVES
	fi
}

# Copy logs to usb for DEBUG
copy_logs() {
	mount | grep "on $1 type" > /dev/null
	DATE=`date`
	if [ $? -eq 0 ]
	then
		echo "Error with mount point"
	else
		`cd $MOUNT_PT/; tar cvf debug_$DATE.tar.gz debug/`
		
		rm -rf $MOUNT_PT/debug/
		mkdir $MOUNT_PT/debug/
		cp -R /var/log/* $MOUNT_PT/debug/
		
		`cd $MOUNT_PT/; tar cvf data_$DATE.tar.gz debug/`
		rm -rf $MOUNT_PT/data/
		mkdir $MOUNT_PT/data/
		cp -R /var/log/* $MOUNT_PT/data/
	fi
}

# Get the mount point of the drive based on the dev name
mount_point_of() {
  $MOUNT_PT=`mount | grep $1 | cut -f3 -d' '`
}

test_for_drives
mount_point_of $DRIVE
copy_logs $DRIVE $MOUNT_PT




