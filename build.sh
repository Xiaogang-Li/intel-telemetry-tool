#!/bin/bash

cd build/lib/
cmake ../../library/
make

cd ../tool/
cmake ../../tool
make
