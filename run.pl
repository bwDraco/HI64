#!/usr/bin/perl
# Made by Rajat Todi 
#
# Assumption is that you have already DOUBLE and INT in the
# directory and also a directory called 'data' inside.
#
#
print `date`;

#
#
print "Running for serial processor on different datatypes...\n";
#
#
print `./DOUBLE > ./data/d1_out`;
print `./INT > ./data/i1_out`;
print `./SHORT > ./data/s1_out`;
print `./FLOAT > ./data/f1_out`;
print `./LONGLONG > ./data/l1_out`;
#
#
print "FINISH FOR SERIAL \n";
