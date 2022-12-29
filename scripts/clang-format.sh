#!/bin/bash

set -e

if [[ $# -gt 0 ]]; then
  CLANG_FORMAT=${1}
  shift;
else
  CLANG_FORMAT=clang-format
fi

if [ ! $(which $CLANG_FORMAT) ]; then
  echo "Please install $CLANG_FORMAT or use the proper argument"
  exit 1
fi

git ls-tree -r $(git branch --show-current) --name-only $(git rev-parse --show-toplevel) |grep "\.[ch]pp$" |xargs $CLANG_FORMAT -i
