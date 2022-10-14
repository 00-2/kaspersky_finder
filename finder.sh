#!/bin/bash
if [[ $# -eq 0 ]] ; then
    echo "Wrong count of element. Required>=1 : Received: "$#
    exit 0
fi
filename="$1"
if [ "$filename" = "~" ]; then
  filename=$(echo ~)
fi
res="$(sudo /usr/lib/finder/finder_bin "$filename")"
if ! [ "$res" = "" ]; then
  echo "$res"
fi