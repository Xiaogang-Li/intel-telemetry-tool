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
    Bao, Bruce <bruce.bao@intel.com>
    Cui, Yao <yao.cui@intel.com>
    Tang, Jun <jun.tang@intel.com>
    Qu, Pengfei <pengfei.qu@intel.com>
    Wang, Binghu <binghu.wang@intel.com>
    Wang, Adeli <adeli.wang@intel.com>
    Wang, Wenjie <wenjie.wang@intel.com>
    Lin, Ben Y <ben.y.lin@intel.com>
    Ma, Kevin <kevin.ma@intel.com>
    Li, Liang A <liang.a.li@intel.com>
