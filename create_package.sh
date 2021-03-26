#!/bin/bash
# run via build_and_action.sh, $1 will be VERSION, $2 will be DOCKER_FILE_SUFFIX (e.g. focal)
set -euo pipefail
fpm \
    -t deb \
    -s dir \
    -C /output \
    -p /build \
    --depends libboost-date-time-dev \
    --depends libboost-system-dev \
    --depends libboost-thread-dev \
    --depends libboost-dev \
    --name kin_logfmt \
    --version $1_$2 \
    usr

