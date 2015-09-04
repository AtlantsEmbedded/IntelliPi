#!/bin/sh /etc/rc.common
# MindBx startup script
 
START=1
STOP=2
 
start() {        

	hciconfig hci0 sspmode 1

	/atom/data/data_interface /atom/data/config/data_config.xml &
	sleep 2
	/atom/data/data_preprocessing /atom/data/config/preprocess_config.xml &
	sleep 2
	/atom/app/mindbx_app /atom/app/config/application_config.xml &
}                 
 
stop() {          
        killall data_interface
        killall data_preprocessing
        killall mindbx_app
}

boot() {
	hciconfig hci0 sspmode 1

	/atom/data/data_interface /atom/data/config/data_config.xml &
	sleep 2
	/atom/data/data_preprocessing /atom/data/config/preprocess_config.xml &
	sleep 2
	/atom/app/mindbx_app /atom/app/config/application_config.xml &
	
}
