extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <iostream>
#include <cstdlib>

using namespace std;

int* create_rbtree_data(int size, int seed)
{
  srand(seed);
  int* data = new int[size];
  for (int i = 0; i < size; i++) {
    data[i] = rand();
  }
  return data;
}

lua_State* lua_create_context(const char* file) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, file);
  return L;
}

void lua_rbtree_create(int* contents, int size, lua_State* L)
{
  // first, call the function to create a tree
  lua_getglobal(L, "RBtree");
  int t = lua_getfield(L, -1, "new");
  lua_call(L, 0, 1);
  // next, insert all the elements one by one
  for (int i = 0; i < size; i++) {
    lua_getfield(L, -1, "insert");
    lua_pushvalue(L, -2); // the "self" reference needed for a `:` function
    lua_pushinteger(L, contents[i]);
    lua_call(L, 2, 0);
  }
}

bool lua_rbtree_find(int value, lua_State* L)
{
  lua_getfield(L, -1, "search");
  lua_pushvalue(L, -2);
  lua_pushinteger(L, value);
  lua_call(L, 2, 1);
  bool b = lua_toboolean(L, -1);
  lua_pop(L, 1);
  return b;
}

int main()
{
  int size = 10000;
  // first, generate test data
  int* data = create_rbtree_data(size, 0);
  // load lua code and create a state
  lua_State* L = lua_create_context("rb-tree.lua");
  // create the RB tree, leaving it as the top value on the stack, this can be timed
  lua_rbtree_create(data, size, L);

  // do some cleanup
  lua_close(L);
  delete [] data;
}
