mkdir build
cd build
mkdir docs
cmake -DCANNON_BUILD_DOC=ON ..
make doc_doxygen
