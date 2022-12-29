#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  FORMATTER=${1}
  shift;
else
  FORMATTER=black
fi
$(dirname "$0")/format.sh black '-e \.py$' $FORMATTER -l80
