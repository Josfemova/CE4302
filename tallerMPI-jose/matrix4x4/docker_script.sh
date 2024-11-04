#!/bin/sh
machines
cat /root/machinefile 
cd /root/project/sobel/
echo $PWD
mpicc matrix_mul.c -o build/mpi -lm
sync
mpiexec -f /root/machinefile -n 4 build/mpi