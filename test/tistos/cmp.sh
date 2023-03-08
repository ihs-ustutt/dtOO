#!/bin/bash

cWith=$(echo "$1" | sed -e "s%~.*$%.msh%g")
echo "Compare $1 <-> $cWith"
#xzcmp $1 $cWith
diff --expand-tabs --width=150 --suppress-common-lines --side-by-side \
  <(xzcat $1 | sed -e '1,/Nodes/d' -e '/EndNodes/,$d'  | tail -n+2 | awk '{print $2" "$3" "$4}' | sort -n -k1) \
  <(cat $cWith | sed -e '1,/Nodes/d' -e '/EndNodes/,$d' | tail -n+2 | awk '{print $2" "$3" "$4}' | sort -n -k1) \
  > ${1}.diff

if [ -s ${1}.diff ]; then
  # file is not-empty
  exit 1
else
  # file is empty.
  exit 0
fi
