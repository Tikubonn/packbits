
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <packbits/packbits-errno.h>
#include <packbits/packbits-decoder.h>
#include "test.h"

static void test_packbits_decoder_put (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 0);
  TEST(datasize == 0);
}

static void test_packbits_decoder_put2 (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_put(-1, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('a', &decoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == 'a');
  TEST(data[1] == 'a');
  TEST(packbits_decoder_put(-2, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('b', &decoder, data, &datasize) == 0);
  TEST(datasize == 3);
  TEST(data[0] == 'b');
  TEST(data[1] == 'b');
  TEST(data[2] == 'b');
  TEST(packbits_decoder_put(-3, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('c', &decoder, data, &datasize) == 0);
  TEST(datasize == 4);
  TEST(data[0] == 'c');
  TEST(data[1] == 'c');
  TEST(data[2] == 'c');
  TEST(data[3] == 'c');
  TEST(packbits_decoder_put(3, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('d', &decoder, data, &datasize) == 0);
  TEST(datasize == 1);
  TEST(data[0] == 'd');
  TEST(packbits_decoder_put('e', &decoder, data, &datasize) == 0);
  TEST(datasize == 1);
  TEST(data[0] == 'e');
  TEST(packbits_decoder_put('f', &decoder, data, &datasize) == 0);
  TEST(datasize == 1);
  TEST(data[0] == 'f');
  TEST(packbits_decoder_put('g', &decoder, data, &datasize) == 0);
  TEST(datasize == 1);
  TEST(data[0] == 'g');
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 0);
  TEST(datasize == 0);
}

static void test_packbits_decoder_put3 (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_put(-127, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('a', &decoder, data, &datasize) == 0);
  TEST(datasize == 128);
  for (size_t i = 0; i < 128; i++){
    TEST(data[i] == 'a');
  }
  TEST(packbits_decoder_put(-127, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('a', &decoder, data, &datasize) == 0);
  TEST(datasize == 128);
  for (size_t i = 0; i < 128; i++){
    TEST(data[i] == 'a');
  }
  TEST(packbits_decoder_put(-1, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put('a', &decoder, data, &datasize) == 0);
  TEST(datasize == 2);
  for (size_t i = 0; i < 2; i++){
    TEST(data[i] == 'a');
  }
}

static void test_packbits_decoder_put4 (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_put(127, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  for (size_t n = 0; n < 128; n++){
    TEST(packbits_decoder_put(n, &decoder, data, &datasize) == 0);
    TEST(datasize == 1);
    TEST(data[0] == n);
  }
  TEST(packbits_decoder_put(127, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  for (size_t n = 0; n < 128; n++){
    TEST(packbits_decoder_put(n, &decoder, data, &datasize) == 0);
    TEST(datasize == 1);
    TEST(data[0] == n);
  }
  TEST(packbits_decoder_put(1, &decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  for (size_t n = 0; n < 2; n++){
    TEST(packbits_decoder_put(n, &decoder, data, &datasize) == 0);
    TEST(datasize == 1);
    TEST(data[0] == n);
  }
}

static void test_packbits_decoder_put_error (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_decoder_put(1, &decoder, data, &datasize) == 1); //tried to put after finalized.
  TEST(packbits_errno == PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED);
}

static void test_packbits_decoder_finalize_error (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_put(0, &decoder, data, &datasize) == 0);
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 1); //tried to finalize with incomplete input.
  TEST(packbits_errno == PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE);
}

static void test_packbits_decoder_finalize_error2 (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_put((uint8_t)-1, &decoder, data, &datasize) == 0);
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 1); //tried to finalize with incomplete input.
  TEST(packbits_errno == PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE);
}

static void test_packbits_decoder_finalize_error3 (){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 0); 
  TEST(packbits_decoder_finalize(&decoder, data, &datasize) == 1); //tried to finalize after finalized.
  TEST(packbits_errno == PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED);
}

void test_packbits_decoder (){
  test_packbits_decoder_put();
  test_packbits_decoder_put2();
  test_packbits_decoder_put3();
  test_packbits_decoder_put4();
  test_packbits_decoder_put_error();
  test_packbits_decoder_finalize_error();
  test_packbits_decoder_finalize_error2();
  test_packbits_decoder_finalize_error3();
}
