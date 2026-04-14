#!/bin/bash

D="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" >/dev/null 2>&1 && pwd )"


C=$1
B=`basename ${C%%.sv.c}`
W=`dirname ${C}`
PROC="$W/build/$B.proc.sv.c"
IR="$W/build/$B.sv.ll"
BC="$W/build/$B.sv.bc"
ASM="$W/build/$B.sv.asm"


# Frontend
mkdir -p "$W/build/"
if [ $C -nt $IR ]
then
	echo PROC=$PROC
	#TODO R_ω instead R_w.
	#	-H Maybe it generate right code, just warn
	#		-H Suppress warning
	clang -E -x assembler-with-cpp $C -o $PROC
	R=$?
	if (( $R != 0 ))
	then
		exit $R
	fi

	echo IR=$IR

	clang -S -emit-llvm -c $C -o $IR
	R=$?
	if (( $R != 0 ))
	then
		exit $R
	fi

	clang -emit-llvm -c  $C -o $BC
	R=$?
	if (( $R != 0 ))
	then
		exit $R
	fi

fi

function backend() {
	LLC="$1"
	MARCH="$2"
	ASM="$W/build/$B.$MARCH.asm"
	echo ASM=$ASM
	$LLC -march=$MARCH $IR -o $ASM
	R=$?
	if (( $R != 0 ))
	then
		exit $R
	fi
}

if [ $IR -nt $ASM ] || true
then
	echo backend
	backend llc x86-64
	#backend llc x86
	#backend llc arm
	backend llc arm64
	#backend llc riscv32 # Do not want it
	#TODO Set path to compilet cpu0 llc and try to compile
	#backend $D/../SW/LLVM/out/bin/llc cpu0
	#TODO Implement our.
	#backend llc sv
fi