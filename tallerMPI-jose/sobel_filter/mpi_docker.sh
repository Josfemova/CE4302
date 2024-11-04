#!/bin/sh

OUTDIR=../docker-mpi/project/sobel

echo "Este script asume que ya ejecutó 'make test'"
rm -rf $OUTDIR
mkdir $OUTDIR
cp . -r $OUTDIR/../sobel
cp ./docker_script.sh $OUTDIR
cd ../docker-mpi/
cp project/sobel/docker-compose.yml . 
cp attach-stack . 
docker-compose build
docker-compose push 
sh ./start-stack; 
sleep 2;
sh ./attach-stack;
sh ./stop-stack
cd ../sobel_filter
cp $OUTDIR/build/test${TEST}_mpidocker.txt ./build/test${TEST}_mpidocker.txt
rm -rf $OUTDIR
(cd ../docker_mpi; git restore .)

