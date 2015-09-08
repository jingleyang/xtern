CC=g++
XTERN_ROOT=/home/yjy/hku/smt-mc/xtern/
all: clean speedup-example-mutex-only speedup-example-mutex-array 
speedup-example-mutex-only:
	$(CC) speedup-example-mutex-only.cpp -pthread -g -O0 -o speedup-example-mutex-only
speedup-example-mutex-array:
	$(CC) speedup-example-mutex-array.cpp -pthread -g -O0 -o speedup-example-mutex-array
clean:
	rm -rf speedup-example-mutex-only speedup-example
speedup-example:
	$(CC) speedup-example.cpp -pthread -g -O0 -I$(XTERN_ROOT)/include -L$(XTERN_ROOT)/dync_hook -Wl,--rpath,$(XTERN_ROOT)/dync_hook -lxtern-annot  -o speedup-example
native:
	/usr/bin/time -v ./speedup-example-mutex-only
parrot:
	/usr/bin/time -v env LD_PRELOAD=./interpose.so ./speedup-example-mutex-only
array:
	/usr/bin/time -v  ./speedup-example-mutex-array
parrot-array:
	/usr/bin/time -v env LD_PRELOAD='./interpose.so' ./speedup-example-mutex-array
