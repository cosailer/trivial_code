#!/bin/bash

for i in {1..13..1}
do
   cd $i
   mv project3_harmonic.py project3_harmonic_coat.py
   cd .. 
done
