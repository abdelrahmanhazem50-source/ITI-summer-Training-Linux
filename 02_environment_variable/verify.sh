#!/bin/bash

echo "Current shell: $COMPANY"
echo "Child shell  : $(bash -c 'echo $COMPANY')"
echo
 echo "Open a new terminal and run: echo \$COMPANY"
