#!/bin/sh
machines
cat /root/machinefile 
cd /root/project/sobel/
echo $PWD
mpicc parallel_mpi.c -o build/mpi -lm
sync
mpiexec -f /root/machinefile -n 4 build/mpi build/test${TEST}.txt build/test${TEST}_mpidocker.txt