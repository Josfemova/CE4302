default: help

.PHONY: help
help: ## Show help for each of the Makefile recipes. 
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[1m\033[35m%-13s\033[0m %s\n\033[0m", $$1, $$2}'
	@printf "\033[5m\033[1m%-13s\n\033[0m" "Please define the TEST variable when executing test commands!"
	@printf "\033[1m%-13s\033[36m %s\n\033[0m" "Supported TEST values: [1,2,3,4,5]";
	@printf "\033[1m%-13s\033[36m %s\n\033[0m" "Example:" 'make test TEST=1';

build_all: clean build/serial build/omp build/mpi ## build all codes 

clean: ## clean build files
	rm -rf build 
	mkdir build

build/serial: ## compile serial code
	gcc serial.c -o build/serial -lm

build/omp: ## compile code using neon intrinsics
	gcc parallel_omp.c -o build/omp -lm -fopenmp

build/mpi: ## compile cuda code 
	mpicc parallel_mpi.c -o build/mpi -lm 

test: build_all ## executes all codes for one test image, saves result pngs in build folder
	python3 img2file.py -i imagenes/test$(TEST).png -o build/test$(TEST).txt 
	./build/serial build/test$(TEST).txt build/test$(TEST)s.txt
	./build/omp build/test$(TEST).txt build/test$(TEST)omp.txt
	mpiexec -n 4 ./build/mpi build/test$(TEST).txt build/test$(TEST)mpi.txt
	# mpi en docker 
	TEST=$(TEST) sh mpi_docker.sh
	python3 file2img.py -i build/test$(TEST)s.txt -o build/test$(TEST)_serial.png 
	python3 file2img.py -i build/test$(TEST)omp.txt -o build/test$(TEST)_omp.png 
	python3 file2img.py -i build/test$(TEST)mpi.txt -o build/test$(TEST)_mpi.png 
	python3 file2img.py -i build/test$(TEST)mpi.txt -o build/test$(TEST)_mpi_docker.png 
	xdg-open build/test$(TEST)_serial.png
	xdg-open build/test$(TEST)_omp.png
	xdg-open build/test$(TEST)_mpi.png
	xdg-open build/test$(TEST)_mpi_docker.png
