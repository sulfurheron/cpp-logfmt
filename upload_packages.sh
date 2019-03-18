#!/bin/bash
set -euo pipefail
echo "yes" | $HOME/.local/bin/kin_ci upload /build/*.deb
