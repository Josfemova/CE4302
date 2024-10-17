#!/bin/sh
name=test

build: clean
	sh ../assembler/compiler/compile.sh $(name).SISA > output_files/$(name).txt
	python ../assembler/compiler/text2hex_mif.py -i output_files/$(name).txt \
		-o output_files/$(name)

clean: 
	rm -rf output_files
	mkdir output_files