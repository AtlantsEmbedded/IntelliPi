#!/bin/sh /etc/rc.common
# MindBx startup script

START=1
STOP=2

start() {

	hciconfig hci0 sspmode 1

	/intelli/data/data_interface /intelli/data/config/data_config.xml &
	sleep 2
	/intelli/data/data_preprocessing /intelli/data/config/preprocess_config.xml &
	sleep 2
	/intelli/app/braintone_app /intelli/app/config/braintone_app_config.xml &
}

stop() {
        killall data_interface
        killall data_preprocessing
        killall braintone_app
}

boot() {
	hciconfig hci0 sspmode 1

	/intelli/data/data_interface /intelli/data/config/data_config.xml &
	sleep 2
	/intelli/data/data_preprocessing /intelli/data/config/preprocess_config.xml &
	sleep 2
	/intelli/app/braintone_app /intelli/app/config/braintone_app_config.xml &

}
