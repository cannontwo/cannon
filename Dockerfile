FROM ubuntu:bionic

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
        wget

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

RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update && apt-get install -y gcovr \
        libompl-dev \
        clang \
        gcc-9 \
        g++-9

# OMPL
#WORKDIR /
#RUN wget https://ompl.kavrakilab.org/install-ompl-ubuntu.sh
#RUN chmod u+x install-ompl-ubuntu.sh
#RUN ./install-ompl-ubuntu.sh
#WORKDIR /ompl-1.5.0/
#RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/local/
#RUN make install

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9

COPY . /cannon
RUN mkdir -p /cannon/build
WORKDIR /cannon/build
RUN cmake ..
RUN make -j4

ENTRYPOINT ["make", "test"]
