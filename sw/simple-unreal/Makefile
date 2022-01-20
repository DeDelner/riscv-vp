all : main.cpp unreal.cpp
	riscv32-unknown-elf-g++ -std=c++14 main.cpp unreal.cpp -o main -march=rv32i -mabi=ilp32

sim: all
	riscv-vp --intercept-syscalls main

dump-elf: all
	riscv32-unknown-elf-readelf -a main

dump-code: all
	riscv32-unknown-elf-objdump -D main

dump-comment: all
	objdump -s --section .comment main

clean:
	rm -f main 
	rm -f main