#!/bin/sh
# tst.sh

if [ ! -f "lbm.sh" ]; then :
  echo "Must create 'lbm.sh' file (use 'lbm.sh.example' as guide)." >&2
  exit 1
fi

rm -f topicsponge *.log *.pid

###./bld.sh ; if [ $? -ne 0 ]; then exit 1; fi

. ./lbm.sh

# Start Stateful Resolver Service (SRS)
SRS srs.xml >srs.log 2>&1 &
# Wait up to 5 seconds for SRS to create its PID file.
for I in 1 2 3 4 5; do :
  if [ ! -f "srs.pid" ]; then sleep 1; fi
done
if [ ! -f "srs.pid" ]; then echo "`date` SRS fail?" >&2; kill_pids; exit 1; fi
SRS_PID="`cat srs.pid`"; echo "`date` SRS_PID=$SRS_PID"


export LBM_XML_CONFIG_APPNAME="topicsponge"
export LBM_XML_CONFIG_FILENAME="um.xml"

../Linux64/topicsponge >topicsponge.log &
TOPICSPONGE_PID=$!

sleep 1


lbmsrc abc -d 1 -M 1 -L 0

lbmsrc xyz -d 1 -M 1 -L 0

lbmsrc abc -d 1 -M 1 -L 0

kill $TOPICSPONGE_PID $SRS_PID

wc -l topicsponge.log
