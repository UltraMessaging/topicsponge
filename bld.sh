#!/bin/sh
# bld.sh - build.

# Construct table of contents for each page
for F in *.md; do :
  if egrep "<!-- mdtoc-start -->" $F >/dev/null; then :
    # Update doc table of contents (see https://github.com/fordsfords/mdtoc).
    if which mdtoc.pl >/dev/null; then mdtoc.pl -b "" $F;
    elif [ -x ../mdtoc/mdtoc.pl ]; then ../mdtoc/mdtoc.pl -b "" $F;
    else echo "FYI: mdtoc.pl not found; see https://github.com/fordsfords/mdtoc"; exit 1
    fi
  fi
done

if [ ! -f "lbm.sh" ]; then :
  echo "Must create 'lbm.sh' file (use 'lbm.sh.example' as guide)." >&2
  exit 1
fi

. ./lbm.sh

# For Linux
LIBS="-L $LBM/lib -l lbm -l pthread -l m -l rt"

rm -rf topicsponge

gcc -Wall -g -I $LBM/include -I $LBM/include/lbm -o topicsponge topicsponge.c $LIBS
if [ $? -ne 0 ]; then exit 1; fi

mv topicsponge Linux64/
