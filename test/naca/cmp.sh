#!/bin/bash

cWith=$(echo "$1" | sed -e "s%~.*$%.msh%g")
echo "Compare $1 <-> $cWith"
xzcmp $1 $cWith
