#
# Hackit challenge makefile
# Written 2022 by d3phys
#

ld   = /lib64/ld-linux-x86-64.so.2
crt  = /lib64/crt1.o
libc = /lib64/libc.so.6

EXE   = hackit

make: main.o
	ld $^ $(crt) $(libc) -I$(ld) -s -o hackit
	./hackit
	
%.o: %.cpp
	g++ -fno-pie -fno-stack-protector -c $< -o $@

clean:
	rm -f *.o
	rm -f *.lst
	rm -f $(EXE)

.PHONY: clean make
