#############################################################################
AtlantsEmbedded:
Intelligent & Machine Learning Systems

Created by:Alex Joseph
email: josephvalex@gmail.com
#############################################################################

This file describes the steps to add a package locally to an image.

Step 1:

Add an entry in feeds.conf.default file located inside intelliroot directory.
 

Example:
src-link local /home/user/IntelliPi/local

The path for the location of customfeed is given. 


Step 2:

Edit the OpenWRT Makefile inorder to give the path for source code and Makefile corresponding to the source code.
 

In the following example, source code and the Makefile corresponding to the source code resides in myBeeper directory.
Both are copied in to the build directory for the package. 

Example:

define Build/Prepare
        cp -R /home/alexjo/Atlants/IntelliPi/src/myBeeper/* $(PKG_BUILD_DIR)
endef

Step 3: 
Update and install the feeds.

./scripts/feeds update -a
./scripts/feeds install -a

Step 4: 
Run make menuconfig.
Package will be visible after adding all the dependend libraries.

In this case, dependend library is wiringPi, which can be found under the libs category of the Atlants-GPL section.
After adding the library, the package myBeeper(in this case) will be visible under the libs category of the Atlants-GPL section.

