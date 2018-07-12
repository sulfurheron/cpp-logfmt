#!/bin/bash

set -euo pipefail

docker build . -t logfmt:build
docker run -it --rm -v `pwd`:/ws -w /ws logfmt:build bash
