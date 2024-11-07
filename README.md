
# packbits 

![](https://img.shields.io/badge/-C++-blue?logo=cplusplus)
![](https://img.shields.io/badge/license-MIT-blue)
![](https://img.shields.io/badge/version-1.0.0-orange)

packbits は PackBits 形式による圧縮・伸長機能を提供します。

packbits が提供する機能は次のとおりです。

* オンライン・オフラインアルゴリズムによる圧縮・伸長
* `errno.h` が提供する `errno` に類似したエラー通知機能

## Usage

ビルド後に作成される `dist/include` `dist/lib` 内のファイルを適切な場所に配置して使います。 それ以外は標準的なC言語ライブラリと同じように扱います。

```c
#include <stdio.h>
#include <stddef.h>
#include <packbits/packbits.h>

int main (){

  const char plaintext[] = "Hello.";

  size_t encodeddatasize;
  char *encodeddata = packbits_encode(plaintext, sizeof(plaintext) -1, PACKBITS_DEFAULT_REALLOC_FUNC, &encodeddatasize);
  fwrite(encodeddata, encodeddatasize, 1, stdout);
  putc('\n', stdout);

  size_t decodeddatasize;
  char *decodeddata = packbits_decode(encodeddata, encodeddatasize, PACKBITS_DEFAULT_REALLOC_FUNC, &decodeddatasize);
  fwrite(decodeddata, decodeddatasize, 1, stdout);
  putc('\n', stdout);

  return 0;

}
```

```bash
gcc -Idist/include -Ldist/lib example.c -lpackbits
```

## Command 

ビルド後に作成される `bin/packbits.exe` を実行することで、コマンドラインから PackBit 形式の圧縮・伸長機能を利用することができます。

```bash
./bin/packbits
```

## Build 

同梱されている `makefile` を使用することでビルドすることができます。

```bash
make
```

非常に簡素なテストも同梱されています。

```bash
make test test-bin
```

## License 

&copy; 2024 tikubonn

packbits released under the [MIT License](./LICENSE).
