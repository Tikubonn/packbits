
/**
 * @file 
 * @brief packbits_realloc_func, PACKBITS_DEFAULT_REALLOC_FUNC が宣言されています。
 */

#pragma once
#include <stddef.h>

/**
 * @brief packbits.h の関数が要求する、メモリの再割り当てを行う関数のポインタ型です。
 * @param address 再割り当て元のメモリ領域のアドレスです。
 * @param newsize 再割り当てされるサイズです。
 * @return 成功ならば有効なアドレス、失敗ならば NULL を返してください。
 * @note 引数 address が NULL ならば、新規のメモリ領域を割り当ててください。
 * @note 引数 newsize は、元々のメモリ領域よりも小さい値が指定される場合があります。
 */

typedef void *(*packbits_realloc_func)(void *address, size_t newsize);

/**
 * @brief packbits.h の関数に指定することのできる packbits_realloc_func 型の定数です。
 * @note これは stdlib.h が提供する realloc と同じ振る舞いをします。
 * @note この定数を指定して生成されたすべての実行結果は、不要になった際に free() 関数を用いて開放する必要があります。
 */

extern const packbits_realloc_func PACKBITS_DEFAULT_REALLOC_FUNC;
