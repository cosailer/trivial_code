clc
clear
data=importdata('cantilever_1000_quad-harmonic-mechDisplacement-node-2002-top_right.hist');
y=data.data(:,4);
f=data.data(:,1);
loglog(f,y)
