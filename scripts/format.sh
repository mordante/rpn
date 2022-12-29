#!/bin/bash

set -e

if [[ $# -ne 4 ]]; then
	echo "Usage format.sh <name> <pattern> <formatter> <format-args>"
	echo "However this is a helper script that is not intended to be called directly."
fi

if [ ! $(which ${3}) ]; then
  echo "Binary ${3} for ${1} is not found"
  exit 1
fi

git ls-tree -r $(git branch --show-current) --name-only $(git rev-parse --show-toplevel) |grep ${2} |xargs ${3} ${4}
