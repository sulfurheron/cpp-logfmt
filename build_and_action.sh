#!/bin/bash

#
# To build the libraries & executables and exec into the container:
#   ./build_and_action.sh ./exec.sh container_name
#
# To build the libraries & executables and output the package:
#   ./build_and_action.sh ./create_package.sh container_name
#

set -euo pipefail

show_usage()
{
	echo "Usage: ./build_and_action.sh [./exec.sh|./create_package.sh] container_name docker_file"
}

if [ "$#" -ne 3 ]; then
	show_usage && exit 1
fi
docker build . -t $2 -f $3

docker run -it \
    -e AWS_ACCESS_KEY_ID \
    -e AWS_SECRET_ACCESS_KEY \
    -e AWS_DEFAULT_REGION \
    -e CIRCLE_PROJECT_REPONAME \
    -e CIRCLE_BRANCH \
    -e GFKEY \
    -e PACKAGECLOUD_TOKEN \
    --rm -v `pwd`/build:/build $2 \
    $1
