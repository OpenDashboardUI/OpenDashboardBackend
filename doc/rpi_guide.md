# OpenDashboard - Raspberry Pi Guide

This manual explains how to setup a build environment for native building of OpenDashboard and its requirements on a Raspberry Pi (native build, no cross build). 

## Prerequisite

* Raspberry Pi OS/ Raspbian (Tested with Buster)
* Min 8 GB SD-Card
* Flash drive or external Hard-Disk

## Preparation

This will explain how to setup the build environment and the required dependencies. This includes building a native Qt5 with EGLFS hardware acceleration.

### Install tools & dependencies

Useful tools:
	
	sudo apt-get update
	sudo apt-get install git vim tmux fish cmake sshfs dphys-swapfile
	
Libraries:

	sudo apt-get install libts-dev libprotobuf-dev protobuf-compiler libxerces-c-dev libfmt-dev libboost1.67-all-dev libfontconfig1-dev


### rpi-update

Run rpi-update to make sure that the required **EGLFS** libraries are available:

	rpi-update

### raspi-config

Changes in **/boot/config.txt**::
If you're using you're screen upside down, add this line to rotate the screen: 

	lcd_rotate=2
	
To increase the video memory, add this line (you might want to adjust the value):

	gpu_mem=256
	
### Set environment variables

Manual:

	export QT_QPA_EGLFS_FORCE888=1
	export QT_QPA_EGLFS_PHYSICAL_WIDTH=155
	export QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
    
Or permanent (bash): **/etc/environment:**

	QT_QPA_EGLFS_FORCE888=1
	QT_QPA_EGLFS_PHYSICAL_WIDTH=155
	QT_QPA_EGLFS_PHYSICAL_HEIGHT=86

### Mount ssh 

Mount home folder of dev machine to access OpenDashboard sources:

	sshfs <USER>@<IP_ADDRESS>:/home/<USER>/ /media/<USER>

### Mount external drive

Mount an external flash drive or hard-disk for building Qt on it:

	sudo mount /dev/sda1 /mnt/	

### Activate swap

Compiling with multiple cores requires more than the available 1 GB of RAM. Using a swap memory is therefore required. If swapping is neccessary the speed and responsiveness of the system will decrease significantly but it will prevent the system from crashing.

Edit configuration: **/etc/dphys-swapfile**:

	CONF_SWAPSIZE=2048
	CONF_SWAPFILE=/mnt/swap/swap0

Activate swap:

	sudo dphys-swapfile setup
	sudo dphys-swapfile swapon

### Increase video memory

Use the raspi-config tool to set the video memory to 256 MB

	raspi-config
	
### Add user to groups

To use the touch input and display output, the user needs to be added to the following groups:

	sudo adduser USERNAME video
	sudo adduser USERNAME input

**Note:** Not necessary for the standard user.

## Qt5

Tested with Qt-5.15.1. Later versions might require additional changes.

### Get source

	mkdir /mnt/qt
	cd /mnt/qt
	wget http://download.qt.io/official_releases/qt/5.15/5.15.1/single/qt-everywhere-src-5.15.1.tar.xz
	tar xf qt-everywhere-src-5.15.1.tar.xz

### Fix RPi 3 build configuration

The upstream configuration for the RPi3 is broken for somehow. This can be fixed by adjusting the file for the RPi 1 with the compiler options for the RPi 3. 
Replace the **/mnt/qt/qt-everywhere-src-5.15.1/qtbase/mkspecs/devices/linux-rasp-pi3-g++/qmake.conf** file with the following content:

	# qmake configuration for:
	#  - Raspberry PI 3
	# Based on config for
	#  - Raspberry PI
	#  - Raspberry PI Zero
	# This is required because the original RPi 3 config causes issues with EGLFS
	# http://wiki.qt.io/RaspberryPi

	include(../common/linux_device_pre.conf)

	QMAKE_RPATHLINKDIR_POST += $$[QT_SYSROOT]/opt/vc/lib

	QMAKE_LIBDIR_OPENGL_ES2 = $$[QT_SYSROOT]/opt/vc/lib
	QMAKE_LIBDIR_EGL        = $$QMAKE_LIBDIR_OPENGL_ES2
	QMAKE_LIBDIR_OPENVG     = $$QMAKE_LIBDIR_OPENGL_ES2

	QMAKE_INCDIR_EGL        = $$[QT_SYSROOT]/opt/vc/include \
		                  $$[QT_SYSROOT]/opt/vc/include/interface/vcos/pthreads \
		                  $$[QT_SYSROOT]/opt/vc/include/interface/vmcs_host/linux
	QMAKE_INCDIR_OPENGL_ES2 = $${QMAKE_INCDIR_EGL}
	QMAKE_INCDIR_OPENVG     = $${QMAKE_INCDIR_EGL}

	QMAKE_LIBS_EGL          = -lEGL -lGLESv2
	QMAKE_LIBS_OPENVG       = -lEGL -lOpenVG -lGLESv2

	QMAKE_INCDIR_BCM_HOST   = $$[QT_SYSROOT]/opt/vc/include
	QMAKE_LIBDIR_BCM_HOST   = $$[QT_SYSROOT]/opt/vc/lib
	QMAKE_LIBS_BCM_HOST     = -lbcm_host

	contains(DISTRO, squeeze) {
	    #Debian Squeeze: Legacy everything
	    QMAKE_LIBS_OPENGL_ES2   = -lGLESv2 -lEGL
	} else:contains(DISTRO, arch) {
	    #On principle: no wizardry required
	} else {
	    #This is not strictly necessary
	    DISTRO_OPTS += deb-multi-arch
	    DISTRO_OPTS += hard-float
	}

	QMAKE_CFLAGS            = -march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8
	QMAKE_CXXFLAGS          = $$QMAKE_CFLAGS

	# Preferred eglfs backend
	EGLFS_DEVICE_INTEGRATION = eglfs_brcm

	include(../common/linux_arm_device_post.conf)

	load(qt_config)



### Build Qt5

Configure Qt in embedded mode (no X11 required):

Create out of source tree build folder:

	mkdir /mnt/qt/build_qt-everywhere-src-5.15.1
	cd /mnt/qt/build_qt-everywhere-src-5.15.1
	
Configure:

	../qt-everywhere-src-5.15.1/configure -v \
		-opengl es2 \
		-tslib \
		-device linux-rasp-pi3-g++ \
		-device-option CROSS_COMPILE=/usr/bin/ \
		-opensource -confirm-license \
		-optimized-qmake \
		-reduce-exports \
		-release \
		-qt-pcre \
		-make libs \
		-no-use-gold-linker \
		-no-pkg-config \
		-prefix /usr/local/qt5 \
		-skip wayland \
		-skip qtscript

Build: This step will take multiple hours. It is possible to build 4 or 5 jobs but this will also increase the chance of running out of physical memory and therefore swapping. 

	make -j3
	
Install:

	sudo make -j3 install




## Build googletest

Get source and checkout version 1.10:~~

	git clone https://github.com/google/googletest.git 
	cd googletest
	git checkout v1.10.x
	
Build and install:

	mkdir build
	cd  build
	make -j3
	sudo make -j3 install

## Build OpenDashboard

	mkdir ~/OpenDashboardBackend_build/
	cd ~/OpenDashboardBackend_build/
	cmake -DCMAKE_PREFIX_PATH=/usr/local/Qt-5.15.1/ /media/<USER>/<PATH_TO_OPEN_DASHBOARD>

	
## References

	https://wiki.qt.io/Native_Build_of_Qt_5.4.1_on_a_Raspberry_Pi
	http://codecepts.de/en/cross-compiling-qt-5-15-with-qtwebengine-for-raspberry-pi-3/
	https://www.tal.org/tutorials/building-qt-512-raspberry-pi