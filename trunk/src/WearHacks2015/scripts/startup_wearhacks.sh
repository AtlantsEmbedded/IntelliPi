#!/bin/sh /etc/rc.common
# WearHacks specific startup script
 
START=1
STOP=2
 
start() {        
        /atom/data/data_interface config/zwhacks_datainterface_muse_1.xml
        /atom/data/data_interface config/zwhacks_datainterface_muse_2.xml
        /atom/data/data_processing config/zwhacks_preprocessing_muse_1.xml
        /atom/data/data_processing config/zwhacks_preprocessing_muse_2.xml
        /atom/cerebralwars_app
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
