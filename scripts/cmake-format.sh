#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  FORMATTER=${1}
  shift;
else
  FORMATTER=cmake-format
fi
$(dirname "$0")/format.sh cmake-format '-e CMakeLists\.txt$  -e \.cmake$' $FORMATTER -i
