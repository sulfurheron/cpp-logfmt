#!/bin/bash
set -euo pipefail

# get DISTRIB_CODENAME
source /etc/lsb-release

fpm \
    -t deb \
    -s dir \
    -C /mongo-driver \
    --name mongo-cxx-driver \
    --version 3.1.3.$(date -u +%Y%m%d%H%M%S)-$DISTRIB_CODENAME \
    --vendor mongodb \
    --url 'https://github.com/mongodb/mongo-cxx-driver' \
    --license BSD \
    usr
