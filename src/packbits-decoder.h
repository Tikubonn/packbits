
/**
 * @file 
 * @brief ストリーミングアルゴリズムによる PackBits 形式の復号機能を提供します。
 */

#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief packbits_decoder_put(), packbits_decoder_finalize() 関数の引数に指定するメモリ領域の最低限の長さを表す定数です。
 */

#define PACKBITS_DECODER_RESULT_BUFFER_SIZE 128

typedef enum packbits_decoder_state {
  PACKBITS_DECODER_PENDING,
  PACKBITS_DECODER_FINALIZED,
} packbits_decoder_state;

typedef enum packbits_decoder_mode {
  PACKBITS_DECODER_UNDETERMINED,
  PACKBITS_DECODER_REPEATING,
  PACKBITS_DECODER_DISCRETE,
} packbits_decoder_mode;

/**
 * @brief ストリーミングアルゴリズムを用いて、任意の PackBits 形式のデータを平文に復号するための構造体です。
 * @note この構造体は packbits_decoder_setup() 関数を用いて初期化されます。
 */

typedef struct packbits_decoder {
  packbits_decoder_state state;
  packbits_decoder_mode mode;
  size_t repeat;
} packbits_decoder;

/**
 * @brief packbits_decoder 構造体を初期化します。
 * @param decoder 初期化する packbits_decoder 構造体へのポインタ。
 * @note この関数は必ず成功します。
 */

extern void __stdcall packbits_decoder_setup (packbits_decoder *decoder);

/**
 * @brief PackBits 形式で符号化されたデータの一部を入力し、その時点の復号された平文を引数で指定されたメモリ領域に書き込みます。
 * @param byte PackBits形式で符号化されたデータの一部。
 * @param decoder packbits_decoder_setup() 関数で初期化された packbits_decoder 構造体へのポインタ。
 * @param data その時点で復号された平文が書き込まれるメモリ領域へのポインタ。
 * @param datasizep その時点で復号された平文の長さが格納される size_t 整数型へのポインタ。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning 引数 decoder が既に packbits_decoder_finalize() 関数によって操作されていた場合、この関数は失敗します。
 * @warning 引数 byte に不正な値が指定された場合、この関数は失敗します。
 */

extern int __stdcall packbits_decoder_put (uint8_t byte, packbits_decoder *decoder, uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE], size_t *datasizep);

/**
 * @brief PackBits 形式で符号化されたデータの入力を終了し、その時点の復号された平文を引数で指定されたメモリ領域に書き込みます。
 * @param decoder packbits_decoder_setup() 関数で初期化された packbits_decoder 構造体へのポインタ。
 * @param data その時点で復号された平文が書き込まれるメモリ領域へのポインタ。
 * @param datasizep その時点で復号された平文の長さが格納される size_t 整数型へのポインタ。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning 引数 decoder が既に packbits_decoder_finalize() 関数によって操作されていた場合、この関数は失敗します。
 * @warning 引数 decoder が復号中の状態の場合にもこの関数は失敗します。
 */

extern int __stdcall packbits_decoder_finalize (packbits_decoder *decoder, uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE], size_t *datasizep);
