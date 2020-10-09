cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=/usr/include/boost -DBOOST_LIBRARYDIR=/usr/lib/x86_64-linux-gnu ..
make -j4
make test
make Cannon_coverage
cp compile_commands.json ../
cd ..
