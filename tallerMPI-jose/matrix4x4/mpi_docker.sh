#!/bin/sh

OUTDIR=../docker-mpi/project/sobel

rm -rf $OUTDIR
mkdir $OUTDIR
cp . -r $OUTDIR/../sobel
cp ../sobel_filter/docker -r $OUTDIR/../sobel/
cp ./docker_script.sh $OUTDIR
cd ../docker-mpi/
cp project/sobel/docker/docker-compose.yml . 
cp project/sobel/docker/attach-stack . 
docker-compose build
docker-compose push 
sh ./start-stack
sleep 2;
sh ./attach-stack
sh ./stop-stack
cd ../matrix4x4
rm -rf $OUTDIR
(cd ../docker-mpi; git restore .)

