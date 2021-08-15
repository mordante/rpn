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
            'lines' : 5,
            'branches' : 4,
            'regions': 3,
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
            kind = 'Explicit'
    elif lines != expected['lines'] or branches != expected['branches'] or regions != expected['regions']:
        kind = 'Implicit'

    if kind != None:
        passed = False
        print('{}\n\t{}\n\tlines = {}\n\tbranches = {}\n\tregions = {}'
            .format(filename, kind, lines, branches, regions))

if not passed:
    exit(1)
