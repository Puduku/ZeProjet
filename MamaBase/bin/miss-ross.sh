#!/bin/bash
#####################################
# THE KATZENJAMMER KIDS : Miss Ross #
#####################################

ret=0
ReportProblem () {
  echo "Invalid $1 configuration: $2"  > /dev/stderr
  ret=1
}

echo "Checking Mama configuration..."

if [ -z "${MAMA}" ] ;then 
  ReportProblem Mama "MAMA env var not set" 
else
  if [ ! -d "${MAMA}" ] ;then
    ReportProblem Mama "MAMA ($MAMA) position not found"
  fi
fi   
if [ $ret -ne 0 ] ;then
  exit $ret
fi

for util in coco progress loquet realpath ;do
  if ! which "$util" > /dev/null ;then
    ReportProblem Mama "$util command not found"     
  fi
done
if [ $ret -eq 0 ] ;then
  echo "Mama is detected and configured (Position : $MAMA)."
else
  exit $ret
fi


if [ -z "${FLINT}" ] ;then
  echo "Flint is not configured (FLINT env var not set)" 
else
  if [ -d "${FLINT}" ] ;then
    echo "Flint is detected and configured (Position : $FLINT)."
  else
    ReportProblem Flint "FLINT ($FLINT) position not found"
  fi
fi   

if [ $ret -eq 0 ] ;then
  echo "No problem detected."
  echo "(But it's about time that der Captain beats the carpets...)"
fi
exit $ret

