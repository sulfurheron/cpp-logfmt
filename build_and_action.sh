#!/bin/bash

# Usage: ./build_and_action.sh [bash|./create_package.sh]

set -euo pipefail

docker build . -t logfmt:build
docker run -it --rm -v `pwd`:/logfmt logfmt:build $1
