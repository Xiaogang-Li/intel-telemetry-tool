# intel-telemetry-tool
a library for measuring the state of intel devices (CPU, GPU, Memory, etc.)

This tool has some dependencies, please install the following dependencies before build this tool.

	• libdrm https://anongit.freedesktop.org/git/mesa/drm.git
	• libva  https://github.com/intel/libva.git
	• Intel media driver https://github.com/intel/media-driver
	• OpenCL https://github.com/intel/compute-runtime/blob/master/documentation/BUILD_Ubuntu.md
	
	
Library and Tool build:
$ cd build/lib
$ cmake ../../library
$ make

$ cd build/tool
$ cmake ../../tool
$ make


Or you can run build.sh directly.


![screenshot](https://github.com/Xiaogang-Li/intel-telemetry-tool/blob/master/screenshot.png)
