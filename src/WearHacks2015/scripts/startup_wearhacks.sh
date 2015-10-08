#!/bin/sh /etc/rc.common
# WearHacks specific startup script

START=1
STOP=2

start() {
      #  /Atlants/data/data_interface config/zwhacks_datainterface_muse_1.xml &
      #  /Atlants/data/data_interface config/zwhacks_datainterface_muse_2.xml &
      #  /Atlants/data/data_processing config/zwhacks_preprocessing_muse_1.xml &
      #  /Atlants/data/data_processing config/zwhacks_preprocessing_muse_2.xml &
      #  /Atlants/cerebralwars_app
}

stop() {
        killall data_interface
        killall data_processing
        killall cerebralwars_app
}

restart() {
		stop
		start
}

boot() {
       start
}
