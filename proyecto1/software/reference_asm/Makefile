#!/bin/sh

build: clean
	sh ../assembler/compiler/compile.sh reverb.S > output_files/reverb.txt
	python ../assembler/compiler/text_to_mif.py -i output_files/reverb.txt \
		-o output_files/reverb.mif

clean: 
	rm -rf output_files
	mkdir output_files