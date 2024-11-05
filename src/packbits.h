
/**
 * @file 
 * @brief オフラインアルゴリズムによる PackBits 形式の符号化・復号機能を提供します。
 */

#pragma once
#include <stdint.h>
#include <stddef.h>
#include "packbits-realloc-func.h"

/**
 * @brief 平文を PackBits 形式で符号化します。
 * @param data 符号化するデータへのポインタです。
 * @param datasize 符号化するデータの長さです。
 * @param realloc_func 符号化済みデータを保存する領域を割り当てる際に使われる packbits_realloc_func 型の関数です。
 * @param compdatasizep 符号化されたデータのの大きさが書き込まれる size_t 型のポインタです。
 * @return 成功ならば、符号化されたデータが書き込まれた領域の先頭アドレスを返します。
 */

extern void __stdcall *packbits_encode (const void *data, size_t datasize, packbits_realloc_func realloc_func, size_t *compdatasizep);

/**
 * @brief PackBits 形式で符号化されたデータを平文に復号します。
 * @param data 復号するデータへのポインタです。
 * @param datasize 復号するデータの長さです。
 * @param realloc_func 復号済みでデータを保存する領域を割り当てる際に使われる packbits_realloc_func 型の関数です。
 * @param compdatasizep 復号されたデータのの大きさが書き込まれる size_t 型のポインタです。
 * @return 成功ならば、復号されたデータが書き込まれた領域の先頭アドレスを返します。
 */

extern void __stdcall *packbits_decode (const void *data, size_t datasize, packbits_realloc_func realloc_func, size_t *decompdatasizep);
