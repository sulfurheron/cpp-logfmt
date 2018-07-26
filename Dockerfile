FROM ubuntu:xenial

RUN apt-get update \
        && apt-get install -y build-essential ruby-dev rubygems cmake \
        && gem install fpm \
        && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
        && apt-get install -y --no-install-recommends \
           libgtest-dev \
           libboost-date-time-dev \
           libboost-system-dev \
           libboost-thread-dev \
           libboost-dev \
        && rm -rf /var/lib/apt/lists/*

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
RUN /logfmt/usr/bin/logger_test

WORKDIR /logfmt/
