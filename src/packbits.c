
#include <stdint.h>
#include <stddef.h>
#include "buffer.h"
#include "packbits.h"
#include "packbits-errno.h"
#include "packbits-realloc-func.h"

#ifndef MIN
#define MIN(a, b) ((a)<(b)?(a):(b))
#endif

void __stdcall *packbits_encode (const void *data, size_t datasize, const packbits_realloc_func realloc_func, size_t *compdatasizep){
  buffer buffer;
  buffer_setup(&buffer);
  for (size_t index = 0; index < datasize;){
    if (index +1 < datasize){
      if (((uint8_t*)data)[index] == ((uint8_t*)data)[index +1]){ //put as repetition.
        size_t index_repetition = index +1;
        while (index_repetition < datasize && ((uint8_t*)data)[index] == ((uint8_t*)data)[index_repetition]){
          index_repetition++;
        }
        size_t count_repetition = index_repetition - index;
        for (size_t index_chunk = 0; index_chunk < count_repetition;){
          size_t increment = MIN(count_repetition - index_chunk, 128);
          if (buffer_put(1 - increment, realloc_func, &buffer) || 
              buffer_put(((uint8_t*)data)[index], realloc_func, &buffer)){
            packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
            return NULL;
          }
          index_chunk += increment;
        }
        index += count_repetition;
      }
      else { //put as discrated.
        size_t index_discrated = index +1;
        while (index_discrated < datasize){
          if (((uint8_t*)data)[index_discrated] != ((uint8_t*)data)[index_discrated -1]){
            index_discrated++;
          }
          else {
            index_discrated--;
            break;
          }
        }
        size_t count_discrated = index_discrated - index;
        for (size_t index_chunk = 0; index_chunk < count_discrated;){
          size_t increment = MIN(count_discrated - index_chunk, 128);
          if (buffer_put(increment -1, realloc_func, &buffer) || 
              buffer_write(data + index + index_chunk, increment, realloc_func, &buffer)){
            packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
            return NULL;
          }
          index_chunk += increment;
        }
        index += count_discrated;
      }
    }
    else { //put remainder as repetition.
      if (buffer_put(0, realloc_func, &buffer) || 
          buffer_put(((uint8_t*)data)[index], realloc_func, &buffer)){
        packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
        return NULL;
      }
    }
  }
  if (buffer_strip(realloc_func, &buffer)){
    buffer_free(&buffer);
    packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
    return NULL;
  }
  void *compdata;
  buffer_data(&buffer, &compdata, compdatasizep);
  return compdata;
  // buffer buffer;
  // buffer_setup(&buffer);
  // size_t index = 0;
  // while (index < datasize){
  //   if (index +1 < datasize){
  //     if (((uint8_t*)data)[index] == ((uint8_t*)data)[index +1]){ //repetition.
  //       size_t index2 = index +1;
  //       while (index2 < datasize && ((uint8_t*)data)[index] == ((uint8_t*)data)[index2]){
  //         index2++;
  //       }
  //       size_t count = index2 - index;
  //       size_t index3 = 0;
  //       while (index3 < count){
  //         size_t increment = MIN(128, count - index3);
  //         if (buffer_put(1 - increment, realloc_func, &buffer) || 
  //             buffer_put(((uint8_t*)data)[index], realloc_func, &buffer)){
  //           buffer_free(&buffer);
  //           packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
  //           return NULL;
  //         }
  //         index3 += increment;
  //       }
  //       index += count;
  //     }
  //     else { //discrated.
  //       size_t index2 = index +2;
  //       while (index2 < datasize && ((uint8_t*)data)[index2] != ((uint8_t*)data)[index2 -1]){
  //         index2++;
  //       }
  //       size_t count = index2 - index;
  //       size_t index3 = 0;
  //       while (index3 < count){
  //         size_t increment = MIN(128, count - index3);
  //         if (buffer_put(increment -1, realloc_func, &buffer) || 
  //             buffer_write(data + index + index3, increment, realloc_func, &buffer)){
  //           buffer_free(&buffer);
  //           packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
  //           return NULL;
  //         }
  //         index3 += increment;
  //       }
  //       index += count +1;
  //     }
  //   }
  //   else { //remainder as repetition.
  //     if (buffer_put(0, realloc_func, &buffer) || 
  //         buffer_put(((uint8_t*)data)[index], realloc_func, &buffer)){
  //       buffer_free(&buffer);
  //       packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
  //       return NULL;
  //     }
  //     index++;
  //   }
  // }
  // if (buffer_strip(realloc_func, &buffer)){
  //   buffer_free(&buffer);
  //   packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
  //   return NULL;
  // }
  // void *compdata;
  // buffer_data(&buffer, &compdata, compdatasizep);
  // return compdata;
}

void __stdcall *packbits_decode (const void *data, size_t datasize, const packbits_realloc_func realloc_func, size_t *decompdatasizep){
  buffer buffer;
  buffer_setup(&buffer);
  size_t index = 0;
  while (index < datasize){
    int8_t repeat = ((int8_t*)data)[index];
    if (0 <= repeat){ //discrated.
      size_t actual_repeat = repeat +1;
      if (index + actual_repeat < datasize){
        for (size_t inner_index = 0; inner_index < actual_repeat; inner_index++){
          if (buffer_put(((int8_t*)data)[index + inner_index +1], realloc_func, &buffer)){
            buffer_free(&buffer);
            packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
            return NULL;
          }
        }
        index += actual_repeat +1;
      }
      else {
        buffer_free(&buffer);
        packbits_errno = PACKBITS_ERRNO_INCOMPLETE_INPUT;
        return NULL;
      }
    }
    else 
    if (-128 == repeat){ //skip.
      index += 1;
    }
    else { //repetition.
      if (index +1 < datasize){
        size_t actual_repeat = 1- repeat;
        for (size_t count = 0; count < actual_repeat; count++){
          if (buffer_put(((int8_t*)data)[index +1], realloc_func, &buffer)){
            buffer_free(&buffer);
            packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
            return NULL;
          }
        }
        index += 2;
      }
      else {
        buffer_free(&buffer);
        packbits_errno = PACKBITS_ERRNO_INCOMPLETE_INPUT;
        return NULL;
      }
    }
  }
  if (buffer_strip(realloc_func, &buffer)){
    buffer_free(&buffer);
    packbits_errno = PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION;
    return NULL;
  }
  void *decompdata;
  buffer_data(&buffer, &decompdata, decompdatasizep);
  return decompdata;
}
