#!/bin/bash

usage () { 
  echo "example:"
  echo "checkRun.sh -l checkRun.log  -t 1000 -i machine.xml -o machineSave.xml -c OpenFOAMCase"
}

#
# options
#
# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
_time=
_out=
_xmlIn=
_xmlOut=
_case=
if [ $# -eq 0 ]
then
  echo "No arguments supplied"
  exit 1
fi
while getopts "i:o:t:sl:hc:" opt; do
    case "$opt" in
    h)  usage
        exit
        ;;
    l)  _out=$OPTARG
        ;;
    i)  _xmlIn=$OPTARG
        ;;
    o)  _xmlOut=$OPTARG
        ;;
    t)  _time=$OPTARG
        ;;
    c)  _case=$OPTARG
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
echo "= checkRun.sh"
echo "=   _time = $_time"
echo "=   _out  = $_out"
echo "=   _case = $_case"
echo "=     @: $@"
echo "="
# echo "= possible states:"
# echo "= run"
# echo "= psuccess: finish parallel run, but no serial result (reconsturct)"
# echo "= success"
# echo "= fail"
# echo "= dfail: fail in dtOO including gmsh"

#
# make out file empty
#
if [ ! -z "$_out" ]; then
  echo "clear output file"
  > $_out
fi

if [ ! -z "$_xmlIn" ] && [ ! -z "$_xmlOut" ]; then
  _states=`listState --xmlIn=${_xmlIn} --xmlOut=${_xmlOut}`
else
  _states=`listState`
fi

_allC=0
_failC=0
_runC=0
_successC=0
_noDirC=0

for aState in ${_states}
do
  echo "$aState"
  _stateString=
	((_allC+=1))

	#
	# running
	#
  # add comma that state X1_1 and X1_10 are not equal
  echo "  > check if running"
  _run=`qstat -x | grep -F "${aState},"`
  if [ ! -z "$_run" ]; then
    _stateString="run"
  fi

	if [ -d "${_case}_${aState}" ] && [ -z "$_stateString" ]; then
    echo "  > directory ${_case}_${aState} exists"
    #
    # successful run
    #
		if [ -d "${_case}_${aState}/$_time" ]; then
      _stateString="success"
    #
    # successful parallel run
    #
		elif [ -d "${_case}_${aState}/processor0/$_time" ]; then
      _stateString="psuccess"
    #
    # check if still running
    #
 		elif [ -f "${_case}_${aState}/checkMesh.log" ]; then
			_ofErrLog=
			_ofErrLog=$_ofErrLog`cat ${_case}_${aState}/checkMesh.log | grep -F "***Boundary openness"`
			_ofErrLog=$_ofErrLog`cat ${_case}_${aState}/checkMesh.log | grep -F "***Open cells found"`
			_ofErrLog=$_ofErrLog`cat ${_case}_${aState}/checkMesh.log | grep -F "***Error"`
			if [ ! -z "$_ofErrLog" ]; then
				_stateString="openboundfail"
			fi
    fi
  fi

	#
	# check log files if no state string yet
	#
  if [ -z "$_stateString" ]; then
		echo "  > get log file names"
		_logFile=`find -maxdepth 1 -type f -name "*${aState}.*"`      
		echo "  > _logFile=$_logFile"
		if [ ! -z "$_logFile" ]; then
			_errLog=
			for aLog in $_logFile
			do
				_errLog=$_errLog`cat $aLog | grep -F "[ E ]"`          
				_errLog=$_errLog`cat $aLog | grep -F "A self-intersection"`
        _errLog=$_errLog`cat $aLog | grep -F "ERROR"`
			done
			echo "  > _errLog=$_errLog"
      if [[ "$_errLog" == *"A self-intersection"* ]]; then
        _stateString="ifail"
      elif [[ "$_errLog" == *"[ E ]"* ]]; then
        _stateString="dfail"
      else
        _stateString="ofail"
      fi
		#
		# failed
		#   
		else
      _stateString="fail"
		fi
  fi

	case "$_stateString" in
		success)  ((_successC+=1))
				;;
		psuccess)  ((_successC+=1))
				;;
		run)  ((_runC+=1))
				;;
		fail)  ((_failC+=1))
				;;
		dfail)  ((_failC+=1))
				;;
		ofail)  ((_failC+=1))
				;;
		ifail)  ((_failC+=1))
				;;
    openboundfail)  ((_failC+=1))
				;;
		noDir)  ((_noDirC+=1))
				;;
    *)  echo "  > unhandled _stateString=$_stateString"
	esac
	echo "  >> $_stateString"
	echo ""
	if [ ! -z "$_out" ]; then
  	echo -e "${aState}\t${_stateString}" >> $_out
 	fi
done

#
# print summary
#
_failPercent=`echo "100*$_failC/$_allC" | bc -l`
_noDirPercent=`echo "100*($_noDirC)/$_allC" | bc -l`
_successPercent=`echo "100*($_successC)/$_allC" | bc -l`
_runPercent=`echo "100*($_runC)/$_allC" | bc -l`
echo "="
echo "= SUMMARY"
echo "="
header="%-10s %8s %11s\n"
format="%-10s %8i %11.2f\n"

printf "$header" "=" "NUM" "PERCENT"
#echo "= num  = $_allC" 
#echo "= numFails = $_failC ($_failPercent)" 
#echo "= numNoDir = $_noDirC ($_noDirPercent)"
#echo "= numRun = $_runC ($_runPercent)"
#echo "= numSuccess = $_successC ($_successPercent)"
printf "$format" \
	"=all" "$_allC" "100.00" \
	"=fails" "$_failC" "$_failPercent" \
	"=noDir" "$_noDirC" "$_noDirPercent" \
	"=run"   "$_runC" "$_runPercent" \
	"=success" "$_successC" "$_successPercent"
