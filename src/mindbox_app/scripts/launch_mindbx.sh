#!/bin/bash

cd DATA_interface/data-daemon/
./data_interface &
cd ..
cd ..
cd DATA_preprocessing/preprocess-daemon/
./data_preprocessing &
cd ..
cd ..
cd mindbox_app/
./mindbox_app &
cd ..

ps aux | grep -i data_interface
ps aux | grep -i data_preprocessing
ps aux | grep -i data_mindbox
