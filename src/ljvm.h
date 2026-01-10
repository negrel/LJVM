#ifndef _LJVM_H
#define _LJVM_H

#include <stddef.h>
#include <stdint.h>

/**
 * LuaJIT virtual machine.
 */
struct ljvm;

/**
 * A function that can be run by the VM.
 */
struct ljvm_func;

/*
 * Bytecode instruction format, 32 bit wide, fields of 8 or 16 bit:
 *
 * +----+----+----+----+
 * | B  | C  | A  | OP | Format ABC
 * +----+----+----+----+
 * |    D    | A  | OP | Format AD
 * +--------------------
 * MSB               LSB
 *
 * In-memory instructions are always stored in host byte order.
 */
typedef uint32_t ljvm_bc;

/**
 * Bytecode register.
 */
typedef uint32_t ljvm_bc_reg;

/**
 * Bytecode position.
 */
typedef uint32_t ljvm_bc_pos;

/**
 * Bytecode line number.
 */
typedef uint32_t ljvm_bc_line;

/**
 * A memory-allocation function. On failure, a NULL pointer MUST be returned.
 *
 * If `nsize` is 0 allocate function should free `ptr`.
 * If `osize` is 0 allocate function should allocate `nsize` bytes of memory.
 * Otherwise, `ptr` should be reallocated. If a reallocation fails, `ptr`
 * memory should not be released.
 */
typedef void *(*ljvm_alloc)(void *ud, void *ptr, size_t osize, size_t nsize);

/**
 * Allocates and initialize a new LuaJIT virtual machine. On failure, a NULL
 * pointer is returned.
 */
struct ljvm *ljvm_new(ljvm_alloc alloc, void *alloc_ud);

/**
 * Destroy all objects in the given LuaJIT virtual machine and frees all
 * dynamic memory used by this VM.
 */
struct ljvm *ljvm_destroy(struct ljvm *L);

/**
 * Allocates and initializes a new LuaJIT function.
 */
struct ljvm_func *ljvm_func_new(struct ljvm *L);

/**
 * Emit single bytecode instruction in function `f`.
 */
void ljvm_func_emit(struct ljvm_func *f, ljvm_bc bcode);

/**
 * Destroy the function and all associated dynamic memory.
 */
struct ljvm_func *ljvm_func_destroy(struct ljvm *L);

#define LJVM_BC_ABC(o, a, b, c)                                                \
  (((ljvm_bc)(o)) | ((ljvm_bc)(a) << 8) | ((ljvm_bc)(b) << 24) |               \
   ((ljvm_bc)(c) << 16))
#define LJVM_BC_AD(o, a, d)                                                    \
  (((ljvm_bc)(o)) | ((ljvm_bc)(a) << 8) | ((ljvm_bc)(d) << 16))
#define LJVM_BC_AJ(o, a, j)                                                    \
  LJVM_BC_AD(o, a, (ljvm_bc_pos)((int32_t)(j) + 0x8000))

#endif /* _LJVM_H */
