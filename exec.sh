#!/bin/bash
set -euo pipefail

cd cpp-logfmt/build
# Configure cmake for the directory above
cmake \
  -D CMAKE_BUILD_TYPE=Release \
  -D CMAKE_INSTALL_PREFIX=/output/usr \
  ..

cd ../../kin_logfmt/build
# Configure cmake for the directory above
cmake \
  -D CMAKE_BUILD_TYPE=Release \
  -D CMAKE_INSTALL_PREFIX=/output/usr \
  ..

cd ../..
bash
