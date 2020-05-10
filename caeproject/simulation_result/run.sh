#!/bin/bash

for i in {2..12..1}
do
   cd $i
   echo "N = $i, simulation start "
   nacs  -m ./project3.nmf -p ./project3_harmonic_sub.py -l nacs_mphs  project3
   rm ./project3.nrf  ./results_gid/project3.post.bin
   cd ..
done
