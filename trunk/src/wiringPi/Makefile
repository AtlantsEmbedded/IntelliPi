clean:
	make -C wiringPi clean
	make -C devLib clean
	make -C gpio clean
	make -C examples clean
	make -C examples/Gertboard clean
	make -C examples/PiFace clean
	make -C examples/q2w clean
	make -C examples/PiGlow clean
	
install:
	make -C wiringPi
	make -C wiringPi install
	make -C devLib
	make -C devLib install
	make -C gpio
	make -C gpio install
	mkdir -p /usr/share/wiringPi/examples/Gertboard
	mkdir -p /usr/share/wiringPi/examples/PiFace
	mkdir -p /usr/share/wiringPi/examples/PiGlow
	mkdir -p /usr/share/wiringPi/examples/q2w
	find examples -exec install -m 0644 -D -t /usr/share/wiringPi/examples {} \;
