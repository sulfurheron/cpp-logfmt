#!/bin/bash

#
# To build the libraries & executables and exec into the container:
#   ./build_and_action.sh ./exec.sh
#
# To build the libraries & executables and output the package:
#   ./build_and_action.sh ./create_package.sh
#

set -euo pipefail

show_usage()
{
	echo "Usage: ./build_and_action.sh [./exec.sh|./create_package.sh]"
}

if [ "$#" -ne 1 ]; then
	show_usage && exit 1
fi
./clean.sh
mkdir -p ./kin_logfmt/build
mkdir -p ./cpp-logfmt/build
docker build . -t logfmt:build-arm -f Dockerfile.arm
docker run -it --rm -v `pwd`:/logfmt logfmt:build-arm $1
