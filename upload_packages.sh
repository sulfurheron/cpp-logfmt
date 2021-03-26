#!/bin/bash
# run via build_and_action.sh, $1 will be VERSION (unused), $2 will be DOCKER_FILE_SUFFIX (e.g. focal)
set -euo pipefail
echo "yes" | $HOME/.local/bin/kin_ci upload --deb-distro ubuntu --deb-version $2 /build/*.deb
