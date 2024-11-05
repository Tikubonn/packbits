
/**
 * @file
 * @brief packbits_errno, packbits_errno_message() を提供します。
 */

#pragma once
#include <threads.h>

/**
 * @brief packbits が提供する各種関数が失敗したときに設定される値が列挙されています。
 * @param PACKBITS_ERRNO_NONE 関数は失敗しなかった。
 * @param PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED packbits_encoder は既に packbits_encoder_finalize() 関数によって終了されている。
 * @param PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE packbits_encoder の状態に不整合が発生している。
 * @param PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED packbits_decoder は既に packbits_decoder_finalize() 関数によって終了されている。
 * @param PACKBITS_ERRNO_DECODER_ILLIGAL_STATE packbits_decoder の状態に不整合が発生している。
 * @param PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE packbits_decoder への入力が不完全な状態で packbits_decoder_finalize() 関数が呼び出された。
 * @param PACKBITS_ERRNO_DECODER_INVALID_INPUT packbits_decoder に不正な値が入力された。 
 * @param PACKBITS_ERRNO_INCOMPLETE_INPUT packbits_encode(), packbits_decode() 関数に不完全なデータが渡された。
 * @param PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION packbits_realloc_func 型で指定された関数によるメモリ領域の割り当てが失敗した。
 */

typedef enum _packbits_errno {
  PACKBITS_ERRNO_NONE = 0,
  PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED,
  PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE,
  PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED,
  PACKBITS_ERRNO_DECODER_ILLIGAL_STATE,
  PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE,
  PACKBITS_ERRNO_DECODER_INVALID_INPUT,
  PACKBITS_ERRNO_INCOMPLETE_INPUT,
  PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION
} _packbits_errno;

/**
 * @brief packbits が提供する各種関数が失敗したときに値が設定される変数です。
 * @note この変数は errno.h が提供する errno 変数と同様に、スレッドローカルに振舞います。
 */

extern thread_local _packbits_errno packbits_errno;

/**
 * @brief packbits_errno に設定された値を、読みやすい静的な文字列に変換します。
 * @param err packbits_errno に設定された値です。
 * @note この関数は必ず成功します。
 * @note 未定義の値が指定された場合、この関数は文字列 `"Undefined errno."` を返します。
 */

extern char __stdcall *packbits_errno_message (_packbits_errno err);
