#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  FORMATTER=${1}
  shift;
else
  FORMATTER=clang-format
fi

$(dirname "$0")/format.sh clang-format '-e \.[ch]pp$' $FORMATTER -i
