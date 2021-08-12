#!/usr/bin/env python3

import json
import os

path = os.path.dirname(os.path.abspath(__file__))
path = os.path.join(path, '../')
path = os.path.abspath(path)

not_covered = {
    path + '/src/modules/calculator/controller.cpp' : 
        { 
            'lines' : 0,
            'branches' : 1,
            'regions': 2,
        },
    path + '/src/modules/calculator/model.cpp' : 
        { 
            'lines' : 0,
            'branches' : 3,
            'regions': 0
        }
}

#print(whitelist)

file = open('/data/src/rpn/build/ccov/tests.json')
coverage = json.load(file)
file.close()

passed = True
for file in coverage["data"][0]["files"] :
    #print(file['filename'])
    #print(file['summary']['lines']['covered'] -
    #    file['summary']['lines']['covered'])
    #print(file['summary']['branches']['notcovered'])
    #print(file['summary']['regions']['notcovered'])
    filename = file['filename']
    lines = file['summary']['lines']['covered'] - file['summary']['lines']['covered']
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




#            passed = False
#            print('{}\n\tImplicit\n\tlines = {}\n\t'
#                    'branches = {}\n\tregions = {}\n'
#                .format(filename, lines, branches, regions))
#    elif lines != expected['lines'] or branches != expected['branches'] or regions != expected['regions']:
#            passed = False
#            print('{}\n\tExplicit\n\tlines = {}\n\t'
#                    'branches = {}\n\tregions = {}\n'
#                .format(filename, lines, branches, regions))


if not passed:
    exit(1)
