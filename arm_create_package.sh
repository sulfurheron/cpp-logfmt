#!/bin/bash
set -euo pipefail

# everything but usr/bin
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
    --architecture armhf \
    usr/lib \
    usr/include

