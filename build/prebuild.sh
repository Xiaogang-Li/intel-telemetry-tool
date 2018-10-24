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

install_dir=""

is_ubuntu=$(lsb_release -a | grep "Ubuntu")
if [ "${is_ubuntu}" != "" ]; then
    install_dir="--libdir=/usr/lib/x86_64-linux-gnu"
fi

#Download, build and install lib-drm
#installed=`ldconfig -p | grep libdrm`
if [ "${installed}" = "" ]; then
    echo "Intalling libdrm ..."
    if [ ! -d "drm" ]; then
        try_run 'git clone https://anongit.freedesktop.org/git/mesa/drm.git' 'Failed to intall libdrm.'
    fi
    cd drm
    try_run "./autogen.sh ${install_dir}" 'Failed to intall libdrm.'
    try_run  'make -j8' 'Failed to intall libdrm.'
    try_run 'sudo make install' 'Failed to intall libdrm.'
    cd ../
    echo Done
fi

#Download, build and install libva 
#installed=`ldconfig -p | grep libva`
if [ "${installed}" = "" ]; then
    echo "Intalling libva ..."
    if [ ! -d "libva" ]; then
    try_run  'git clone https://github.com/intel/libva.git' 'Failed to intall libva.'
    fi
    cd libva
    try_run "./autogen.sh ${install_dir}" 'Failed to intall libva.'
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
fi

#Download, build and install OpenCL
installed=`ldconfig -p | grep OpenCL`
if [ "${installed}" = "" ]; then
    echo "Intalling opencl ..."
    mkdir install-opencl >/dev/null 2>&1
    cd install-opencl
    
    sudo apt-get -y install ocl-icd-libopencl1
    sudo apt-get -y install opencl-headers
    sudo apt-get -y install clinfo
    sudo apt-get -y install ocl-icd-opencl-dev
    sudo apt-get -y install beignet
    wget https://github.com/intel/compute-runtime/releases/download/18.41.11649/intel-opencl_18.41.11649_amd64.deb
    sudo dpkg --force-overwrite -i intel-opencl_18.41.11649_amd64.deb

cd ../
echo Done
fi

cd ../
#rm temp -rf

