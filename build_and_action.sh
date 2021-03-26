#!/bin/bash

set -euo pipefail

show_usage()
{
	echo "Usage: ./build_and_action.sh ./exec.sh|./create_package.sh|./arm_create_package.sh CONTAINER_NAME DOCKER_FILE_SUFFIX"
}

if [ "$#" -ne 3 ]; then
	show_usage && exit 1
fi
docker build . -t $2 -f Dockerfile.$3

VERSION="${VERSION:-$(date -u +%Y%m%d%H%M%S)}"

docker run -it \
    -e AWS_ACCESS_KEY_ID \
    -e AWS_SECRET_ACCESS_KEY \
    -e AWS_DEFAULT_REGION \
    -e CIRCLE_PROJECT_REPONAME \
    -e CIRCLE_BRANCH \
    -e GFKEY \
    -e PACKAGECLOUD_TOKEN \
    --rm -v `pwd`/build:/build Dockerfile.$3 \
    $1 $VERSION $3
