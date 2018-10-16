#!/bin/bash
mkdir temp >/dev/null 2>&1
cd temp

function try_run()
{
    command=$1
    message=$2

    $command >/dev/null

    if [ $? -ne 0 ]; then
        echo $message
        exit 1
    fi
}

echo "Installing depend libraries ..."
try_run "sudo apt-get -y install xorg xorg-dev openbox libx11-dev libgl1-mesa-glx libgl1-mesa-dev libncurses5-dev ccache flex bison clang-4.0 cmake g++ git patch zlib1g-dev autoconf xutils-dev libtool pkg-config libpciaccess-dev" "Failed to intall the dependency libraries."
echo Done

#Download, build and install lib-drm
echo "Intalling libdrm ..."
if [ ! -d "drm" ]; then
try_run 'git clone https://anongit.freedesktop.org/git/mesa/drm.git' 'Failed to intall libdrm.'
fi
cd drm
try_run './autogen.sh' 'Failed to intall libdrm.'
try_run  'make -j8' 'Failed to intall libdrm.'
try_run 'sudo make install' 'Failed to intall libdrm.'
cd ../
echo Done

#Download, build and install libva 
echo "Intalling libva ..."
if [ ! -d "libva" ]; then
try_run  'git clone https://github.com/intel/libva.git' 'Failed to intall libdrm.'
fi
cd libva
try_run './autogen.sh' 'Failed to intall libva.'
try_run 'make -j8' 'Failed to intall libva.'
try_run 'sudo make install' 'Failed to intall libva.'
cd ../
echo Done

#Download, build and install intel media driver
echo "Intalling intel media driver ..."
mkdir install-intel-media-driver >/dev/null 2>&1
cd install-intel-media-driver
if [ ! -d "gmmlib" ]; then
try_run 'git clone https://github.com/intel/gmmlib.git' 'Failed to download gmmlib.git.'
fi
if [ ! -d "media-driver" ]; then
try_run 'git clone https://github.com/intel/media-driver.git' 'Failed to download media-driver.git.'
fi

mkdir build >/dev/null 2>&1
cd build
try_run 'cmake ../media-driver' 'Failed to intall intel media driver.'
try_run 'make -j8' 'Failed to intall intel media driver.'
cd ../../
echo Done

#Download, build and install OpenCL
echo "Intalling opencl ..."
mkdir install-opencl >/dev/null 2>&1
cd install-opencl

if [ ! -d "clang_source" ]; then
try_run 'git clone -b release_40 https://github.com/llvm-mirror/clang clang_source' 'Failed to download clang source'
fi
if [ ! -d "common_clang" ]; then
try_run 'git clone https://github.com/intel/opencl-clang common_clang' 'Failed to download opencl-clang'
fi
if [ ! -d "llvm_patches" ]; then
try_run 'git clone https://github.com/intel/llvm-patches llvm_patches' 'Failed to download llvm patches'
fi
if [ ! -d "llvm_source" ]; then
try_run 'git clone -b release_40 https://github.com/llvm-mirror/llvm llvm_source' 'Failed to download llvm'
fi
if [ ! -d "gmmlib" ]; then
try_run 'git clone https://github.com/intel/gmmlib gmmlib' 'Failed to download gmmlib'
fi
if [ ! -d "igc" ]; then
try_run 'git clone https://github.com/intel/intel-graphics-compiler igc' 'Failed to download igc'
fi
if [ ! -d "opencl_headers" ]; then
try_run 'git clone https://github.com/KhronosGroup/OpenCL-Headers opencl_headers' 'Failed to download opencl headers'
fi
if [ ! -d "neo" ]; then
try_run 'git clone https://github.com/intel/compute-runtime neo' 'Failed to download compute-runtime'
fi

mkdir build >/dev/null 2>&1
cd build
try_run 'cmake -DBUILD_TYPE=Release -DCMAKE_BUILD_TYPE=Release ../neo' 'Failed when run cmake'
try_run 'make -j8 package' 'Failed to build the opencl'
try_run 'sudo dpkg -i intel-opencl-*.x86_64-*.deb' 'Failed to install OpenCL'
sudo cp -r ../opencl_headers/CL /usr/include/

cd ../../
echo Done

cd ../
rm temp -rf

