#!/bin/bash
set -euo pipefail

cd cpp-logfmt
# Configure cmake
cmake \
  -D CMAKE_BUILD_TYPE=Release \
  -D CMAKE_INSTALL_PREFIX=/logfmt/usr \
  .

cd ..
bash
