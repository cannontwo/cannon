mkdir build
cd build
mkdir docs
cmake -DCANNON_BUILD_DOC=ON -DCANNON_BUILD_LIBRARY=OFF ..
make
