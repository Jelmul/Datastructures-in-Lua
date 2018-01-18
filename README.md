# Datastructures-in-Lua

in `lua-5.3.4` subdirectory, do
  make linux

in root, do
  g++ -I./lua-5.3.4/src *.cpp ./lua-5.3.4/src/liblua.a -ldl -o benchmark
