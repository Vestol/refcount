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
#ifndef VSTL_REFCOUNT
#define VSTL_REFCOUNT

#include <stddef.h>
#include <stdbool.h>

struct e_allocate_params {
  const void *context;
  int (*constructor)(const void *, void *);
  void (*destructor)(void *);
  int (*clone)(const void *, void *);
};

struct e_clone_params {
  bool deep;
  const void *context;
};

void *e_allocate(size_t size , const struct e_allocate_params *params);
void *e_clone(const void *ptr, bool deep);

void *e_retain(void *ptr);
void e_release(void *ptr);

void *e_malloc(size_t size) __attribute__((weak));
void *e_realloc(void *ptr, size_t size) __attribute__((weak));
void e_free(void *ptr) __attribute__((weak));

#endif
