#!/bin/sh
name=bootrom

compiler_dir = ../../../assembler/compiler

build: clean
	sh $(compiler_dir)/compile.sh $(name).SISA > output_files/$(name).txt
	python $(compiler_dir)/text2hex_mif.py -i output_files/$(name).txt \
		-o output_files/$(name)

clean: 
	rm -rf output_files
	mkdir output_files