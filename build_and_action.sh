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
mkdir -p ./kin_logfmt/build
mkdir -p ./cpp-logfmt/build
docker build . -t logfmt:build

#docker run -it --rm -v `pwd`:/logfmt logfmt:build $1

docker run -it \
    -e AWS_ACCESS_KEY_ID \
    -e AWS_SECRET_ACCESS_KEY \
    -e AWS_DEFAULT_REGION \
    -e CIRCLE_PROJECT_REPONAME \
    -e CIRCLE_BRANCH \
    -e GFKEY \
    -e PACKAGECLOUD_TOKEN \
    --rm -v `pwd`/build:/build logfmt:build \
    $1
