FROM ubuntu:xenial

RUN apt-get update \
	&& apt-get install -y build-essential ruby-dev rubygems cmake \
	&& gem install fpm

RUN apt-get update \
        && apt-get install -y --no-install-recommends \
        libgtest-dev \
        libboost-date-time-dev \
        libboost-dev

RUN mkdir -p /logfmt/usr

# build gtest
WORKDIR /usr/src/gtest
RUN cmake . \
    && make \
    && cp *.a /usr/lib

# build programs
COPY cpp-logfmt/ /logfmt/cpp-logfmt/
WORKDIR /logfmt/cpp-logfmt
RUN cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/logfmt/usr \
    .
RUN make install

COPY kin_logfmt/ /logfmt/kin_logfmt/
WORKDIR /logfmt/kin_logfmt
RUN cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/logfmt/usr \
    .
RUN make install

# Run test
RUN /logfmt/usr/bin/test_logfmt_util

