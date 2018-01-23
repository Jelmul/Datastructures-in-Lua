extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "rb-tree.h"

#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using Clock=std::chrono::high_resolution_clock;

lua_State* lua_create_context(const char* file) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, file);
  return L;
}

enum Alteration { insertion, deletion };

struct Alteration_action {
  int key;
  Alteration action;
};

void create_rbtree_data(int seed, int* insert_data, int insert_data_size, Alteration_action* alteration_data, int alteration_count, int* query_data, int query_count) {
  srand(seed);
  for (int i = 0; i < insert_data_size; i++) {
    insert_data[i] = rand();
  }
  for (int i = 0; i < alteration_count; i++) {
    int type = rand() % 2;
    if (type == 0) { // we are adding a new, random value
      alteration_data[i].action = insertion;
      alteration_data[i].key = rand();
    } else { // we are removing a value which was originally in the structure. Possible bug: it gets removed twice.
      alteration_data[i].action = deletion;
      alteration_data[i].key = insert_data[rand() % insert_data_size];
    }
  }
  for (int i = 0; i < query_count; i++) {
    int orig_ratio = 5;
    int alter_ratio = 2;
    int random_ratio = 3;
    int type = rand() % (orig_ratio + alter_ratio + random_ratio);
    int key;
    if (type < orig_ratio) { // query for value in original structure
      key = insert_data[rand() % insert_data_size];
    } else if (type < alter_ratio) { // query for altered value, either inserted or deleted
      key = alteration_data[rand() % alteration_count].key;
    } else { // query for random value, probably not in structure
      key = rand();
    }
    query_data[i] = key;
  }
}

void dump_rbtree_data(int* insert_data, int insert_data_size, Alteration_action* alteration_data, int alteration_count, int* query_data, int query_count) {
  printf("RBtree data:\n");

  for (int i = 0; i < insert_data_size; i++) {
    printf("insert %d\n", insert_data[i]);
  }
  for (int i = 0; i < alteration_count; i++) {
    if (alteration_data[i].action == insertion) {
      printf("add %d\n", alteration_data[i].key);
    } else {
      printf("remove %d\n", alteration_data[i].key);
    }
  }
  for (int i = 0; i < query_count; i++) {
    printf("query %d\n", query_data[i]);
  }
}

struct Benchmark_result {
  int insert_time;
  int alter_time;
  int query_time;
};

Benchmark_result benchmark_rbtree_lua(int* insert_data, int insert_data_size, Alteration_action* alteration_data, int alteration_count, int* query_data, int query_count) {
  lua_State *L = lua_create_context("rb-tree.lua");

  lua_getglobal(L, "RBtree");
  lua_getfield(L, -1, "new");
  lua_call(L, 0, 1);
  cerr << "lua: starting benchmark\n";
  auto t1 = Clock::now();
  for (int i = 0; i < insert_data_size; i++) {
    lua_getfield(L, -1, "insert");
    lua_pushvalue(L, -2);
    lua_pushinteger(L, insert_data[i]);
    lua_call(L, 2, 0);
  }
  cerr << "lua: insert done\n";
  auto t2 = Clock::now();
  for (int i = 0; i < alteration_count; i++) {
    if (alteration_data[i].action == insertion) {
      lua_getfield(L, -1, "insert");
    } else {
      lua_getfield(L, -1, "del");
    }
    lua_pushvalue(L, -2);
    lua_pushinteger(L, alteration_data[i].key);
    lua_call(L, 2, 0);
  }
  cerr << "lua: alterations done\n";
  auto t3 = Clock::now();
  for (int i = 0; i < query_count; i++) {
    // TODO: check result sanity??
    lua_getfield(L, -1, "search");
    lua_pushvalue(L, -2);
    lua_pushinteger(L, query_data[i]);
    lua_call(L, 2, 1);
    bool b = lua_toboolean(L, -1);
    lua_pop(L, 1);
  }
  cerr << "lua: queries done\n";
  auto t4 = Clock::now();
  Benchmark_result res;
  res.insert_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  res.alter_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();
  res.query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();

  lua_close(L);
  return res;
}

Benchmark_result benchmark_rbtree_cpp(int* insert_data, int insert_data_size, Alteration_action* alteration_data, int alteration_count, int* query_data, int query_count) {
  RBtree tree;
  cerr << "cpp: starting benchmark\n";
  auto t1 = Clock::now();
  for (int i = 0; i < insert_data_size; i++) {
    tree.insert(insert_data[i]);
  }
  cerr << "cpp: insert done\n";
  auto t2 = Clock::now();
  for (int i = 0; i < alteration_count; i++) {
    if (alteration_data[i].action == insertion) {
      tree.insert(alteration_data[i].key);
    } else {
      tree.del(alteration_data[i].key);
    }
  }
  cerr << "cpp: alterations done\n";
  auto t3 = Clock::now();
  for (int i = 0; i < query_count; i++) {
    bool b = tree.search(query_data[i]);
  }
  cerr << "cpp: queries done\n";
  auto t4 = Clock::now();
  Benchmark_result res;
  res.insert_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  res.alter_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();
  res.query_time = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
  return res;
}

void benchmark_rbtree(int n, int seed) {
  printf("Benchmarking n=%d, s=%d....\n", n, seed);
  int insert_data_size = n;
  int alteration_count = n * 0.2;
  int query_count = n * 0.2;

  int* insert_data = new int[insert_data_size];
  Alteration_action* alteration_data = new Alteration_action[alteration_count];
  int* query_data = new int[query_count];
  create_rbtree_data(seed, insert_data, insert_data_size, alteration_data, alteration_count, query_data, query_count);

  //dump_rbtree_data(insert_data, insert_data_size, alteration_data, alteration_count, query_data, query_count);

  Benchmark_result lua_res = benchmark_rbtree_lua(insert_data, insert_data_size, alteration_data, alteration_count, query_data, query_count);
  Benchmark_result cpp_res = benchmark_rbtree_cpp(insert_data, insert_data_size, alteration_data, alteration_count, query_data, query_count);

  printf("Results (n=%d, s=%d):\n", n, seed);
  printf("Insertion:\nlua: %d\ncpp: %d\n", lua_res.insert_time, cpp_res.insert_time);
  printf("Alteration:\nlua: %d\ncpp: %d\n", lua_res.alter_time, cpp_res.alter_time);
  printf("Query:\nlua: %d\ncpp: %d\n", lua_res.query_time, cpp_res.query_time);

  delete [] insert_data;
  delete [] alteration_data;
  delete [] query_data;
}

int main(int argc, char* argv[])
{
  int seed;
  if (argc > 1) {
    seed = atoi(argv[1]);
    printf("Seed passed as argument: %d\n", seed);
  } else {
    seed = time(0);
    printf("No seed passed as argument, using time(0) which is %d right now\n", seed);
  }
  srand(seed);
  int sizes[] = {10, 100, 1000, 10000, 100000, 1000000};
  int num = sizeof(sizes) / sizeof(int);
  int seeds[num];
  for (int i = 0; i < num; i++) {
    seeds[i] = rand();
  }
  for(int i = 0; i < num; i++) {
    benchmark_rbtree(sizes[i], seeds[i]);
  }
}
