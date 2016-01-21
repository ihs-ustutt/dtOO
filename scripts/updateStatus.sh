#!/bin/bash

usage () { 
  echo "example:"
  echo "updateStatus.sh -i machine.xml -o machineSave.xml"
}

#
# options
#
# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
_xmlIn=
_xmlOut=

while getopts "i:o:h" opt; do
    case "$opt" in
    h)  usage
        exit
        ;;
    i)  _xmlIn=$OPTARG
        ;;
    o)  _xmlOut=$OPTARG
        ;;
    \?)
      echo "Invalid option: -$OPTARG"
      exit 0
      ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

#
# output options
#
echo "="
echo "= updateStatus.sh"
echo "=   _xmlIn  = $_xmlIn"
echo "=   _xmlOut = $_xmlOut"
echo "=     @: $@"
echo "="

if [ ! -z "$_xmlIn" ] && [ ! -z "$_xmlOut" ]; then
  _states=`listState --xmlIn=runner.xml --xmlOut=runnerSave.xml`
  _cases=`listLabel --xmlIn=runner.xml  --xmlOut=runnerSave.xml --type=case`
else
  _states=`listState`
  _cases=`listLabel --type=case`
fi

for aState in ${_states}
do
	echo "$aState"

	for aCase in ${_cases}
	do
    echo "  > check case ${aCase}"
    if [ -d "${aCase}_${aState}" ]
    then
      echo "  > directory ${aCase}_${aState} exists"
			if [ -f "${aCase}_${aState}/resultValue" ]
			then
        echo "  > file ${aCase}_${aState}/resultValue exists"
        echo "  >> 0"
        echo "0" > ${aCase}_${aState}/status
			else
        echo "2" > ${aCase}_${aState}/status
        echo "  >> 2"
      fi
   fi
	done
done