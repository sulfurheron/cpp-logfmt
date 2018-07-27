#!/bin/bash
set -euo pipefail

fpm \
    -t deb \
    -s dir \
    -C /output \
    --name kin_logfmt \
    --version $(date -u +%Y%m%d%H%M%S) \
    usr
