# Datastructures-in-Lua
### Files
benchmark.cpp: run a benchmark on both datastructures, comparing Lua with C++

rb-tree.h and rb-tree.cpp: C++ implementation of red-black tree

rb-tree.lua: Lua implementation of red-black tree

priority-queue.lua: Lua implementation of priority queue 

For the C++ implementation of priority queue we used an existing implementation.

### How to run a benchmark
In `lua-5.3.4` subdirectory, do

```make linux```

In root, do

```g++ -g -I./lua-5.3.4/src *.cpp ./lua-5.3.4/src/liblua.a -ldl -o benchmark```

You might get an error indicating that you need to include another argument
