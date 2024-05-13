#!/bin/bash

rm -fr tests/files
mkdir -p tests/files
echo "test
This is a tet file
abcdefg
123test456
test123d
äü" > tests/files/in

echo "this is in_01" > tests/files/in_01
echo "this is in_02" > tests/files/in_02

echo "no permissions" > tests/files/no_perm
chmod 000 tests/files/no_perm
