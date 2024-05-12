#!/bin/sh
# tst.sh

if [ ! -f "lbm.sh" ]; then :
  echo "Must create 'lbm.sh' file (use 'lbm.sh.example' as guide)." >&2
  exit 1
fi

rm -f topicsponge *.log

./bld.sh ; if [ $? -ne 0 ]; then exit 1; fi

. ./lbm.sh

if [ -f "topicsponge.xml" ]; then :
  export LBM_XML_CONFIG_APPNAME="topicsponge"
  export LBM_XML_CONFIG_FILENAME="topicsponge.xml"
elif [ -f "topicsponge.cfg" ]; then :
  export LBM_DEFAULT_CONFIG_FILE="topicsponge.cfg"
fi


Linux64/topicsponge >topicsponge.log &
TOPICSPONGE_PID=$!

sleep 1

lbmsrc abc -d 1 -M 1 -L 0

lbmsrc xyz -d 1 -M 1 -L 0

lbmsrc abc -d 1 -M 1 -L 0

kill $TOPICSPONGE_PID

wc -l topicsponge.log
