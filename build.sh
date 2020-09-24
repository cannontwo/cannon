rm -rf build
mkdir build
cd build
cmake -DBOOST_ROOT=/usr/include/boost -DBOOST_LIBRARYDIR=/usr/lib/x86_64-linux-gnu ..
make
cp compile_commands.json ../
cd ..
make -C build test
