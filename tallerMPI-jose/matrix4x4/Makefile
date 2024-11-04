default: help

.PHONY: help
help: ## Show help for each of the Makefile recipes. 
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[1m\033[35m%-13s\033[0m %s\n\033[0m", $$1, $$2}'

clean: ## limpiar espacio de trabajo 
	rm -rf build
	mkdir build 

build: clean matrix_mul.c ## compilar
	mpicc matrix_mul.c -o build/matrix 

run: build ## correr algoritmo en nucleos físicos locales
	mpiexec -n 4 ./build/matrix

run_docker: build ## correr algoritmo en contenedores de docker, 4 nodos
	sh mpi_docker.sh