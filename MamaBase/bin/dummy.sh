#!/bin/bash

cpt=0
while [ $cpt -lt 1000 ] ;do
  sleep 0.01
  let "cpt++"
  echo "cpt:$cpt"
done
