mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCANNON_BUILD_DOC=ON ..
make doc_doxygen
