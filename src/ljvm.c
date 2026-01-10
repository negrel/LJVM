#include "ljvm.h"
#include "lua.h"

typedef lua_State ljvm_thread;

struct ljvm {
  ljvm_thread *T;
};

struct ljvm *ljvm_new(ljvm_alloc allocf, void *allocd) {
  ljvm_thread *T;
  T = lua_newstate(allocf, allocd);
  return (struct ljvm *)T;
}

void ljvm_close(struct ljvm *L) { lua_close(L->T); }
