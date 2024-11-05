
/**
 * @file
 * @private
 * @brief packbits.c で処理済みのデータを蓄えるための機能を提供します。
 * @warning このヘッダファイルは packbits.h 専用に書かれたものであり、外部からインクルードする行為は推奨されません。
 */

#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "packbits-realloc-func.h"

#define MIN_BUFFER_SIZE 128

#ifndef MAX
#define MAX(a, b) ((a)<(b)?(b):(a))
#endif

typedef struct buffer {
  uint8_t *data;
  size_t size;
  size_t seek;
} buffer;

static inline void buffer_setup (buffer *buffer){
  buffer->data = NULL;
  buffer->size = 0;
  buffer->seek = 0;
}

/**
 * @brief buffer に１バイト分のデータを書き込みます。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note buffer に書き込むための領域が不足していた場合、この関数は realloc_func を呼び出して、追加のメモリ領域を確保します。
 * @warning 追加のメモリ領域を確保するために呼び出した realloc_func が失敗した場合、この関数も失敗します。
 */

static inline int buffer_put (uint8_t c, const packbits_realloc_func realloc_func, buffer *buffer){
  if (buffer->size <= buffer->seek){
    size_t extendedsize = MAX(MIN_BUFFER_SIZE, buffer->size * 2);
    void *extendeddata = realloc_func(buffer->data, extendedsize);
    if (extendeddata == NULL){
      return 1;
    }
    buffer->data = extendeddata;
    buffer->size = extendedsize;
  }
  buffer->data[buffer->seek] = c;
  buffer->seek++;
  return 0;
}

/**
 * @brief buffer に任意長のデータを書き込みます。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @note buffer に書き込むための領域が不足していた場合、この関数は realloc_func を呼び出して、追加のメモリ領域を確保します。
 * @warning 追加のメモリ領域を確保するために呼び出した realloc_func が失敗した場合、この関数も失敗します。
 */

static inline int buffer_write (const uint8_t *data, size_t datasize, const packbits_realloc_func realloc_func, buffer *buffer){
  if (buffer->size <= (buffer->seek + datasize)){
    size_t extendedsize = buffer->size;
    while (extendedsize <= (buffer->seek + datasize)){
      extendedsize = MAX(MIN_BUFFER_SIZE, buffer->size * 2);
    }
    void *extendeddata = realloc_func(buffer->data, extendedsize);
    if (extendeddata == NULL){
      return 1;
    }
    buffer->data = extendeddata;
    buffer->size = extendedsize;
  }
  for (size_t i = 0; i < datasize; i++){
    buffer->data[buffer->seek + i] = data[i];
  }
  buffer->seek += datasize;
  return 0;
}

/**
 * @brief buffer が確保しているメモリ領域の大きさを、書き込まれたデータの長さに切り詰めます。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning メモリ領域を調節するために呼び出した realloc_func が失敗した場合、この関数も失敗します。
 */

static inline int buffer_strip (const packbits_realloc_func realloc_func, buffer *buffer){
  if (buffer->seek != buffer->size){
    void *strippeddata = realloc_func(buffer->data, buffer->seek);
    if (0 < buffer->seek && strippeddata == NULL){
      return 1;
    }
    buffer->data = strippeddata;
    buffer->size = buffer->seek;
    return 0;
  }
  else {
    return 0;
  }
}

static inline void buffer_data (buffer *buffer, void **datap, size_t *datasizep){
  *datap = buffer->data;
  *datasizep = buffer->size;
}

static inline void buffer_free (buffer *buffer){
  free(buffer->data);
}
