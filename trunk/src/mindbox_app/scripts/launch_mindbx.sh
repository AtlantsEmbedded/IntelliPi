#!/bin/ash

hciconfig hci0 sspmode 1
/atom/data/data_interface /atom/data/config/data_config.xml &
/atom/data/data_preprocessing /atom/data/config/preprocess_config.xml &
/atom/app/mindbox_app /atom/app/config/application_config.xml &


# #!/bin/sh /etc/rc.common

#START=1
#STOP=2
 
#start() {        
#        hciconfig hci0 sspmode 1
#        /atom/data/data_interface /atom/data/config/data_config.xml &
#        /atom/data/data_preprocessing /atom/data/config/preprocess_config.xml &
#        /atom/app/mindbox_app /atom/app/config/application_config.xml &
#}                 
 
#stop() {          
#        killall data_interface
#        killall data_preprocessing
#        killall data_mindbox
#}

#boot() {
#        hciconfig hci0 sspmode 1
#        /atom/data/data_interface /atom/data/config/data_config.xml &
#        /atom/data/data_preprocessing /atom/data/config/preprocess_config.xml &
#        /atom/app/mindbox_app /atom/app/config/application_config.xml &
#}


