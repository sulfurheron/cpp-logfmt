#!/bin/bash
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
    --version $1 \
    usr

