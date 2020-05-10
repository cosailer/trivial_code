#!/bin/bash

for i in {1..13..1}
do
   cd $i
   rm -rf history results_gid project3.nmf project3.info.xml project3.simlog project3.las
   cd .. 
done
