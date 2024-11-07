#!/bin/bash

cd `dirname $0`
source ./test.bash

test_command 'echo -n "Hello." | packbits | packbits -d' 'Hello.'
test_command 'echo -n "Hello." | packbits -e | packbits -d' 'Hello.'
