# intel-telemetry-tool
This is a library for measuring the state of intel devices (CPU, GPU, Memory, etc.)

### Dependencies

This library has some dependencies, please install the following dependencies before build it.

libdrm: https://anongit.freedesktop.org/git/mesa/drm.git

libva :  https://github.com/intel/libva.git

Intel media driver: https://github.com/intel/media-driver

OpenCL: https://github.com/intel/compute-runtime/blob/master/documentation/BUILD_Ubuntu.md

### Build
    Library and Tool build:
    $ cd build/lib
    $ cmake ../../library
    $ make
    
    $ cd build/tool
    $ cmake ../../tool
    $ make


Or you can run the build.sh directly.

### Screenshot
![screenshot](https://github.com/Xiaogang-Li/intel-telemetry-tool/blob/master/screenshot.png)

### Contributers
    Bao, Bruce
    Cui, Yao
    Tang, Jun
    Qu, Pengfei
    Wang, Binghu
    Wang, Adeli
    Wang, Wenjie
    Lin, Ben Y
    Li, Liang A
    Ma, Kevin
    Li, Xiaogang
