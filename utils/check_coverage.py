#!/usr/bin/env python3

# Copyright (C) Mark de Wever <koraq@xs4all.nl>
# Part of the RPN project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.
#
# See the COPYING file for more details.

# Simple helper tool to validate whether the current coverage matches the
# expected coverage. It validates whether the notcovered numbers didn't change.
# Every change to these numbers causes the script to fail.

import json
import os

path = os.path.dirname(os.path.abspath(__file__))
path = os.path.join(path, '../')
path = os.path.abspath(path)

# The expected numbers of not covered code. All files not in this list are
# expected to have full coverage.
not_covered = {
    path + '/src/modules/calculator/controller.cpp' :
        {
            'lines' : 14,
            'branches' : 11,
            'regions': 6,
        },
    path + '/src/modules/calculator/math/arithmetic.cpp' :
        {
            'lines' : 6,
            'branches' : 1,
            'regions': 0,
        },
    path + '/src/modules/calculator/math/core.cpp' :
        {
            'lines' : 3,
            'branches' : 0,
            'regions': 1,
        },
    path + '/src/modules/calculator/stack.cpp' :
        {
            'lines' : 0,
            'branches' : 1,
            'regions': 0,
        },
    # Note consteval can't be covered
    path + '/src/modules/lib/dictionary.cpp' :
        {
            'lines' : 16,
            'branches' : 0,
            'regions': 2,
        },
    path + '/src/modules/lib/binary_find.cpp' :
        {
            'lines' : 0,
            'branches' : 0,
            'regions': 1,
        },
    path + '/src/modules/parser/detail/base.cpp' :
        {
            'lines' : 2,
            'branches' : 3,
            'regions': 2,
        },
    path + '/src/modules/parser/detail/base.cpp' :
        {
            'lines' : 2,
            'branches' : 3,
            'regions': 2,
        },
    path + '/src/modules/parser/detail/floating_point.cpp' :
        {
            'lines' : 4,
            'branches' : 3,
            'regions': 2,
        },
    path + '/src/modules/parser/detail/signed.cpp' :
        {
            'lines' : 7,
            'branches' : 5,
            'regions': 4,
        },
    path + '/src/modules/parser/detail/string.cpp' :
        {
            'lines' : 1,
            'branches' : 1,
            'regions': 1,
        },
    path + '/src/modules/parser/detail/unsigned.cpp' :
        {
            'lines' : 10,
            'branches' : 7,
            'regions': 4,
        },
    path + '/src/modules/parser/token.cpp' :
        {
            'lines' : 1,
            'branches' : 0,
            'regions': 1,
        },
    # Is this a false positive?
    path + '/tests/lib/binary_find.cpp' :
        {
            'lines' : 26,
            'branches' : 0,
            'regions': 1,
        },
}

file = open(path + '/build/ccov/tests.json')
coverage = json.load(file)
file.close()

passed = True
for file in coverage["data"][0]["files"] :
    filename = file['filename']
    # There's no notcovered for lines.
    lines = file['summary']['lines']['count'] - file['summary']['lines']['covered']
    branches = file['summary']['branches']['notcovered']
    regions = file['summary']['regions']['notcovered']
    expected = not_covered.get(filename)
    kind = None
    if expected == None:
        if lines != 0 or branches != 0 or regions != 0:
            kind = 'implicit'
    elif lines != expected['lines'] or branches != expected['branches'] or regions != expected['regions']:
        kind = 'Explicit'

    if kind != None:
        passed = False
        print('{}\n\t{}\n\tlines = {}\n\tbranches = {}\n\tregions = {}'
            .format(filename, kind, lines, branches, regions))

if not passed:
    exit(1)
