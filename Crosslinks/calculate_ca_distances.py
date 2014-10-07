#! /usr/bin/env python
from math import sqrt

from Bio.PDB.PDBParser import PDBParser
import argparse

parser = argparse.ArgumentParser(description='Calculate distances between specific atoms in pdb')
parser.add_argument('-pdb', action="store", dest="pdbfile", help="the pdb file" )
parser.add_argument('-r1', action="store", dest="residue1", help="the first residue number" )
parser.add_argument('-r2', action="store", dest="residue2", help="the second residue number" )
parser.add_argument('-at1', action="store", dest="atom1", help="the first atom type" )
parser.add_argument('-at2', action="store", dest="atom2", help="the second atom type" )

parser=parser.parse_args()

pdbparser = PDBParser()

structure = pdbparser.get_structure(parser.pdbfile,parser.pdbfile)

dss='''115 59 
59 5 
1034 946 
191 239 
544 488 
278 239 
506 59 
1072 1035 
1072 1034 
1072 936 
168 191 
488 442 
171 191 
532 488 
913 946 
966 963 
973 976 
946 912'''.split("\n")

Xlinks=[]
for xl in dss:
   xlt=xl.split(" ")
   Xlinks.append((int(xlt[0]),int(xlt[1])))
#print Xlinks


edc='''103 130 
103 131 
1072 1032 
1142 1068 
1142 1069 
1144 1068 
168 131 
171 127 
171 195 
171 197 
187 127 
394 454 
480 63 
484 442 
486 442 
506 562 
912 1097 
913 1097 
913 989 
936 392 
936 952 
946 1097 
946 935'''.split("\n")

for xl in edc:
   xlt=xl.split(" ")
   Xlinks.append((int(xlt[0]),int(xlt[1])))
#print Xlinks


model = structure[0]
chain = model[' ']

for link in Xlinks:
    r1,r2=link

    # Get residue numbers
    residue1 = chain[r1+1]
    residue2 = chain[r2+1]

    # Get some atoms
    #ca1 = residue1['CA']
    #ca2 = residue2['CA']
    ca1 = residue1['NZ']
    ca2 = residue2['NZ']
    # Simply subtract the atoms to get their distance
    distance = ca1-ca2

    print r1, r2, distance




