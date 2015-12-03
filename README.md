# IntelliPi
The IntelliPi is designed as a general purpose low-cost EEG-BMI system. We have made it easy to use either the OpenBCI or MUSE EEG headsets, and have easy General-Purpose Input/Output(GPIO) so you no longer need to worry about the hardware nuts and bolts when you want to interact with a button or sensor  - you can just create!
Our software and tools help to auto-generate device drivers so that plugging in a component is almost as easy as plugging a mouse into a computer. We took care of getting the data into the PI, so you can focus on the development of your application.
IntelliPi's seamless data-collection and configuration APIs makes it perfect for any embedded Brain Computing Interfaces (BCIs) applications.

# What can I do?
Have you heard of brain computing interfaces (BCIs), the emerging field of neurotechnology or machine learning?  You don't need to have to go to a university or work with expensive clinical equipment to research, explore or create stand-alone projects that help us change the way we interact with our environment or create autonomous systems capable of self-learning.  Using the infamous and popular RaspberryPi, the IntelliPi allows you to interact with sensors and other electronics just like the regular RaspberryPi, BUT - now you can use a Electroencephalogram or EEG to interact with your world in new ways like playing games or monitoring your neural activity to learn about your brain.  

We have simplified the data-collection process, created open-source user-friendly machine learning libraries, removed the hardware details and have packaged our software into an easy to use ALL-in-ONE solution so you can begin writng appliations and not waste time figuring out everything from scratch!

# How it works
The IntelliPi solution consists of a few components:
* Customized OpenWRT-based Linux distribution which optimizes the synergy between the software and the RaspberryPi
* A software layer for EEG signal processing that runs on any unix-based system
* RaspiBerryPi - A+ or B+
* EEG - MUSE headband or OpenBCI

Using Open-WRT - a Debian-based Linux distribution typically used for small consumer routers, we took advantage of its highly-efficient background for its impressive software ecosystem, enhanced Linux and cross compiling buildroot to build a user-friendly and high-performance Linux image for the IntelliPi.  Sounds complicated, but it's not. Truth is that we took care in configuring a toolchain that compiles Linux and your software and turns it into an image that can then simply be used on your RaspBerryPi, by the mean of an SD card. Again, we made it easy so that you can focus on the realization of your idea.

# Features
To develop you application, we packed in a series of tools to make sure you have all you need to get started, this includes:
* EEG data preprocessing (Moving window frames, Fourier transform, power-band)
* Single and multiple device pairing
* Statistical analysis
* Linear Algebra
* Machine Learning (discriminant analysis, support-vector machine, neural network and more...)
* Data/Features/Label recording to persistent USB storage
* Easy GPIO access (wiringPi)

# We enable - YOU imagine
Combining over 15 years of software and hardware experience - we removed all of the tricky stuff with hardware, connecting to devices and collecting data so that all you need to do is create an application using our APIs to recieve data.  In 4 steps, just install our software, create your applicaton, build an image and go; It is that simple!

# For more information:
Before getting started, read over our <a href="https://github.com/AtlantsEmbedded/IntelliPi/wiki"> WIKI for our quickstart guide, architecture information and more</a>.
