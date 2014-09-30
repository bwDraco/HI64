#!/usr/bin/sh -f
# Made by Rajat Todi 
#
# Assumption is that you have already DOUBLE and INT in the
# directory and also a directory called 'data' inside.
#
#
date
#
#
echo "Running for serial processor on different datatypes..."
#
#
./DOUBLE > ./data/d1_out
./INT > ./data/i1_out
./SHORT > ./data/s1_out
./FLOAT > ./data/f1_out
./LONGLONG > ./data/l1_out
#
#
echo "FINISH FOR SERIAL"
