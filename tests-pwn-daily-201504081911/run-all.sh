#!/usr/bin/env bash

mkdir -p .o asm exe actual

rm actual/*

for src in *.pwn; do
    bname="$(basename -s .pwn "$src")"
    asmfile="asm/$bname.asm"
    ofile=".o/$bname.o"

    echo "Compiling $bname"
    ../pwn -g "$src" -o "$asmfile" || (echo "compilation failure" && continue)
    yasm -gdwarf2 -felf32 "$asmfile" -o "$ofile" || (echo "yasm error" && continue)
    ld -m elf_i386 -L"$HOME/compiladores/root/usr/lib" "$ofile" -lrts -o "exe/$bname" || (echo "ld error" && continue)
    
    "exe/$bname" > "actual/$bname.out" 
    diff --ignore-all-space "expected/$bname.out" "actual/$bname.out" 1>/dev/null || echo "$bname did not pass the tests"
done

rm -r .o

