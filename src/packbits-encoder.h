
/**
 * @file 
 * @brief ストリーミングアルゴリズムによるPackBits形式の符号化機能を提供します。
 */

#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief packbits_encoder_put(), packbits_encoder_finalize() 関数の引数に指定するメモリ領域の最低限の長さを表す定数です。
 */

#define PACKBITS_ENCODER_RESULT_BUFFER_SIZE 129

typedef enum packbits_encoder_state {
  PACKBITS_ENCODER_PENDING,
  PACKBITS_ENCODER_FINALIZED,
} packbits_encoder_state;

typedef enum packbits_encoder_mode {
  PACKBITS_ENCODER_UNDETERMINED,
  PACKBITS_ENCODER_REPEATING,
  PACKBITS_ENCODER_DISCRETE,
} packbits_encoder_mode;

/**
 * @brief ストリーミングアルゴリズムを用いて、任意の平文を PackBits 形式に符号化するための構造体です。
 * @note この構造体は packbits_encoder_setup() 関数を用いて初期化されます。
 */

typedef struct packbits_encoder {
  packbits_encoder_state state;
  packbits_encoder_mode mode;
  uint8_t data[128];
  size_t seek;
} packbits_encoder;

/**
 * @brief packbits_encoder 構造体を初期化します。
 * @param encoder 初期化する packbits_encoder 構造体へのポインタ。
 * @note この関数は必ず成功します。
 */

extern void __stdcall packbits_encoder_setup (packbits_encoder *encoder);

/**
 * @brief 平文の一部を入力し、その時点の PackBits 形式で符号化されたデータを引数で指定されたメモリ領域に書き込みます。
 * @param byte 平文の一部。
 * @param encoder packbits_encoder_setup() 関数で初期化された packbits_encoder 構造体へのポインタ。
 * @param data その時点で符号化されたデータが書き込まれるメモリ領域へのポインタ。
 * @param datasizep その時点で符号化されたデータの長さが格納される size_t 整数型へのポインタ。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning 引数 encoder が既に packbits_encoder_finalize() 関数によって操作されていた場合、この関数は失敗します。
 */

extern int __stdcall packbits_encoder_put (uint8_t byte, packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep);

/**
 * @brief 平文の入力を終了し、その時点の PackBits 形式で符号化されたデータを引数で指定されたメモリ領域に書き込みます。
 * @param decoder packbits_decoder_setup() 関数で初期化された packbits_decoder 構造体へのポインタ。
 * @param data その時点で符号化されたデータが書き込まれるメモリ領域へのポインタ。
 * @param datasizep その時点で符号化されたデータの長さが格納される size_t 整数型へのポインタ。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning 引数 encoder が既に packbits_encoder_finalize() 関数によって操作されていた場合、この関数は失敗します。
 */

extern int __stdcall packbits_encoder_finalize (packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep);
