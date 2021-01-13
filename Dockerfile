FROM ubuntu:bionic AS base

ARG build_graphics=1

RUN apt-get update && apt-get install -y software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt-get update && apt-get install -y build-essential \ 
        cmake \
        libeigen3-dev \
        clang-tidy \
        libcgal-dev \ 
        libglfw3-dev \
        libfreetype6 \
        libfreetype6-dev \
        assimp-utils \
        libassimp-dev \
        libboost-all-dev \
        git \
        wget \
        gcovr \
        libompl-dev \
        clang \
        gcc-9 \
        g++-9 \
        libglvnd0 \
        libgl1 \
        libglx0 \
        libegl1 \
        libxext6 \
        libx11-6 \
        libglvnd-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev \
        pkg-config \
        fonts-open-sans \
        doxygen

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9

# YAML-Cpp
WORKDIR /
RUN wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.0.tar.gz
RUN tar xzvf yaml-cpp-0.6.0.tar.gz
WORKDIR yaml-cpp-yaml-cpp-0.6.0/
RUN mkdir build
WORKDIR build
RUN cmake -DBUILD_SHARED_LIBS=ON ..
RUN make && make install
WORKDIR /

# Catch2
RUN git clone https://github.com/catchorg/Catch2.git
WORKDIR /Catch2/
RUN git checkout v2.x
RUN mkdir build
WORKDIR build
RUN cmake ..
RUN make && make install
WORKDIR /

# Boost
RUN wget https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.gz
RUN tar xzvf boost_1_74_0.tar.gz
WORKDIR /boost_1_74_0/
RUN ./bootstrap.sh --prefix=/usr/
RUN ./b2
RUN ./b2 install
WORKDIR /

# OMPL
#WORKDIR /
#RUN wget https://ompl.kavrakilab.org/install-ompl-ubuntu.sh
#RUN chmod u+x install-ompl-ubuntu.sh
#RUN ./install-ompl-ubuntu.sh
#WORKDIR /ompl-1.5.0/
#RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/local/
#RUN make install

COPY . /cannon
RUN mkdir -p /cannon/build
WORKDIR /cannon/build
RUN cmake .. -DCANNON_BUILD_GRAPHICS=OFF -DCANNON_BUILD_DOC=OFF
RUN make -j4


# Building with graphics capability
#ENV NVIDIA_VISIBLE_DEVICES all
#ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility
#COPY . /cannon
#RUN mkdir -p /cannon/build
#WORKDIR /cannon/build
#RUN cmake ..
#RUN make -j4

ENTRYPOINT ["make", "test"]
