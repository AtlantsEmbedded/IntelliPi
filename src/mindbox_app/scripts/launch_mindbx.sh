#!/bin/sh /etc/rc.common
# MindBx startup script

START=1
STOP=2

start() {

	hciconfig hci0 sspmode 1

	/Atlants/data/data_interface /Atlants/data/config/data_config.xml &
	sleep 2
	/Atlants/data/data_preprocessing /Atlants/data/config/preprocess_config.xml &
	sleep 2
	/Atlants/app/mindbx_app /Atlants/app/config/application_config.xml &
}

stop() {
        killall data_interface
        killall data_preprocessing
        killall mindbx_app
}

boot() {
	hciconfig hci0 sspmode 1

	/Atlants/data/data_interface /Atlants/data/config/data_config.xml &
	sleep 2
	/Atlants/data/data_preprocessing /Atlants/data/config/preprocess_config.xml &
	sleep 2
	/Atlants/app/mindbx_app /Atlants/app/config/application_config.xml &

}
