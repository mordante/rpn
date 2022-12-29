#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  BLACK=${1}
  shift;
else
  BLACK=black
fi

if [ ! $(which $BLACK) ]; then
  echo "Please install $BLACK or use the proper argument"
  exit 1
fi

git ls-tree $(git branch --show-current) -r --name-only $(git rev-parse --show-toplevel) |grep "\.py$" |xargs $BLACK -l80
