#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  CMAKE_FORMAT=${1}
  shift;
else
  CMAKE_FORMAT=cmake-format
fi

if [ ! $(which $CMAKE_FORMAT) ]; then
  echo "Please install $CMAKE_FORMAT or use the proper argument"
  exit 1
fi

git ls-tree $(git branch --show-current) -r --name-only $(git rev-parse --show-toplevel) |grep -e "CMakeLists\.txt$"  -e "\.cmake$" |xargs $CMAKE_FORMAT -i
