/* 
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.

 * For more information, please refer to <http://unlicense.org>
 */
#include "refcount.h"

#include <stdlib.h>
#include <string.h>

struct int_memptr {
  size_t count;
  size_t size;
  void (*destructor)(void *);
  int (*clone)(const void *, void *);
};

static inline size_t int_get_size(const void *ptr)
{
  return ( (struct int_memptr *) ptr - 1)->size;
}

static int int_clone_constr(const void *orig, void *copy)
{
  (void) memcpy(copy, orig, int_get_size(orig) );
  return 0;
}

void *e_allocate(size_t size, const struct e_allocate_params *params)
{
  struct int_memptr *ptr;

  if ( !(ptr = e_malloc(sizeof *ptr + size) ) )
    return NULL;

  if ( (params->constructor) ) {
    if (params->constructor(params->context, ptr + 1) != 0) {
      e_free(ptr);
      return NULL;
    } 
  }

  *ptr = (struct int_memptr) {
    .count = 1,
    .size = size,
    .destructor = params->destructor,
    .clone = params->clone ? params->clone : int_clone_constr
  };

  return ptr + 1;
}

void e_retain(void *ptr)
{
  ( (struct int_memptr *) ptr - 1)->count++;
}

void e_release(void *ptr)
{
  struct int_memptr *mem;

  mem = (struct int_memptr *) ptr - 1;

  if (--mem->count == 0) {
    if (mem->destructor) {
      mem->destructor(ptr);
    }

    e_free(mem);
  }
}

void *e_clone(const void *ptr)
{
  return e_allocate(int_get_size(ptr), &(struct e_allocate_params) {
    .context = ptr,
    .constructor = ( (struct int_memptr *) ptr - 1)->clone,
    .destructor = ( (struct int_memptr *) ptr - 1)->destructor,
    .clone = ( (struct int_memptr *)  ptr - 1)->clone
  });
}

__attribute__((weak))
void *e_malloc(size_t size)
{
  return malloc(size);
}

__attribute__((weak))
void e_free(void *ptr)
{
  free(ptr);
}
