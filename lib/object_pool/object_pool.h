#ifndef OBJECT_POOL
#define OBJECT_POOL
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  bool is_used;
  void* payload_ptr;
} Object;

typedef struct {
  void* memory;                // raw backing memory
  size_t pool_size;            // number of objects
  size_t payload_size;         // aligned payload size
  size_t object_size;          // aligned Object header size
  size_t object_payload_size;  // header + payload
  size_t bytes_count;          // total bytes allocated
} Pool;

// This is a pointer to a function that takes a payload and argument
typedef void(Pool_func_itr)(void* playload, void* args);

Pool* new_object_pool(const size_t object_size, const size_t pool_size);
void remove_object_pool(Pool* p);
void object_pool_itr(Pool* p, Pool_func_itr* func, void* args);
void* object_pool_borrow(Pool* p);
void object_pool_return(Pool* p, void* obj);

#endif  // !OBJECT_POOL
