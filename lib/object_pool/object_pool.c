#include "./object_pool.h"
#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// TODO: O(1) free list
// TODO: Debug poison patterns
// TODO: Pool reset
// TODO: Optional thread safety
// TODO: Allocation failure telemetry

// Determine the system's strictest alignment requirement
static const size_t ALIGNMENT = alignof(max_align_t);

// return the alined size for clean memory bounderies
size_t get_aligned_size(size_t size) {
  return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

Pool* new_object_pool(const size_t payload_size, const size_t pool_size) {
  assert(payload_size > 0);
  assert(pool_size > 0);

  Pool* p = (Pool*)malloc(sizeof(Pool));
  assert(p != NULL);

  p->payload_size = get_aligned_size(payload_size);
  p->object_size = get_aligned_size(sizeof(Object));
  p->object_payload_size = p->object_size + p->payload_size;
  p->pool_size = pool_size;
  p->bytes_count = pool_size * p->object_payload_size;

  void* ptr = malloc(p->bytes_count);
  assert(ptr != NULL);
  p->memory = ptr;

  Object* obj;
  void* payload_ptr;
  // using uint8 to avoid undefined behaviour when doing pointer
  // arithmetics on void pointer
  uint8_t* base = (uint8_t*)p->memory;
  uint8_t* end = base + p->bytes_count;
  for (size_t offset = 0; offset < p->pool_size; offset++) {
    obj = (Object*)(base + (offset * p->object_payload_size));

    assert((uint8_t*)(obj) >= base);
    assert((uint8_t*)(obj) + p->object_payload_size <= end);
    assert(obj != NULL);

    obj->is_used = false;

    payload_ptr = (void*)((uint8_t*)obj + p->object_size);
    assert((uint8_t*)(payload_ptr) > base);
    assert((uint8_t*)(payload_ptr) + p->payload_size <= end);
    assert(payload_ptr != NULL);

    obj->payload_ptr = payload_ptr;
  }
  return p;
};

void* object_pool_borrow(Pool* p) {
  if (p == NULL || p->memory == NULL) {
    return NULL;
  }
  // loop over the objects and return object
  Object* obj;
  uint8_t* base = (uint8_t*)(p->memory);
  uint8_t* end = base + p->bytes_count;

  for (size_t offset = 0; offset < p->pool_size; offset++) {
    obj = (Object*)(base + (offset * p->object_payload_size));

    assert((uint8_t*)(obj) >= base);
    assert((uint8_t*)(obj) + p->object_payload_size <= end);
    assert(obj != NULL);

    if (!obj->is_used) {
      obj->is_used = true;
      assert(obj->payload_ptr != NULL);
      return obj->payload_ptr;
    }
  }
  return NULL;
};

void object_pool_return(Pool* p, void* payload_ptr) {
  if (p == NULL || p->memory == NULL) {
    return;
  }
  uint8_t* base = (uint8_t*)(p->memory);
  uint8_t* end = base + p->bytes_count;

  assert((uint8_t*)(payload_ptr) > base);
  assert((uint8_t*)(payload_ptr) + p->payload_size <= end);
  assert(payload_ptr != NULL);

  Object* obj = (Object*)((uint8_t*)payload_ptr - p->object_size);

  assert((uint8_t*)(obj) >= base);
  assert((uint8_t*)(obj) + p->object_payload_size <= end);
  assert(obj != NULL);
  assert(obj->is_used == true);

  obj->is_used = false;
};

void remove_object_pool(Pool* p) {
  assert(p != NULL);
  assert(p->memory != NULL);
  free(p->memory);
  free(p);
};

void object_pool_itr(Pool* p, Pool_func_itr* func, void* args) {
  if (p == NULL || p->memory == NULL) {
    return;
  }
  // loop over the objects and return object
  Object* obj;
  uint8_t* base = (uint8_t*)(p->memory);
  uint8_t* end = base + p->bytes_count;

  for (size_t offset = 0; offset < p->pool_size; offset++) {
    obj = (Object*)(base + (offset * p->object_payload_size));

    assert((uint8_t*)(obj) >= base);
    assert((uint8_t*)(obj) + p->object_payload_size <= end);
    assert(obj != NULL);

    if (obj->is_used) {
      assert(obj->payload_ptr != NULL);
      func(obj->payload_ptr, args);
    }
  }
  return;
};
