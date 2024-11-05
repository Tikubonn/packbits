
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
