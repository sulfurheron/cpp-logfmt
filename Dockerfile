FROM ubuntu:xenial

RUN apt-get update \
	&& apt-get install -y build-essential ruby-dev rubygems cmake \
	&& gem install fpm

RUN mkdir -p /logfmt/usr

COPY cpp-logfmt/ /logfmt/cpp-logfmt/

WORKDIR /logfmt/cpp-logfmt
RUN cmake \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/logfmt/usr \
    .
RUN make install

# Run test
RUN /logfmt/usr/bin/test_logfmt