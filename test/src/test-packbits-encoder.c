
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <packbits/packbits-errno.h>
#include <packbits/packbits-encoder.h>
#include "test.h"

static void testcase_packbits_encoder_put (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(datasize == 0);
}

static void testcase_packbits_encoder_put2 (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('b', &encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-1);
  TEST(data[1] == 'a');
  TEST(packbits_encoder_put('b', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('b', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('c', &encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-2);
  TEST(data[1] == 'b');
  TEST(packbits_encoder_put('c', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('c', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('c', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('d', &encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-3);
  TEST(data[1] == 'c');
  TEST(packbits_encoder_put('e', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('f', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_put('g', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(datasize == 5);
  TEST(data[0] == 3);
  TEST(data[1] == 'd');
  TEST(data[2] == 'e');
  TEST(data[3] == 'f');
  TEST(data[4] == 'g');
}

static void testcase_packbits_encoder_put3 (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  for (size_t i = 0; i < 128; i++){
    TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
    TEST(datasize == 0);
  }
  TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-127);
  TEST(data[1] == 'a');
  for (size_t i = 0; i < 127; i++){
    TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
    TEST(datasize == 0);
  }
  TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-127);
  TEST(data[1] == 'a');
  TEST(packbits_encoder_put('a', &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(datasize == 2);
  TEST(data[0] == (uint8_t)-1);
  TEST(data[1] == 'a');
}

static void testcase_packbits_encoder_put4 (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  for (size_t n = 0; n < 128; n++){
    TEST(packbits_encoder_put(n, &encoder, data, &datasize) == 0);
    TEST(datasize == 0);
  }
  TEST(packbits_encoder_put(128, &encoder, data, &datasize) == 0);
  TEST(datasize == 129);
  TEST(data[0] == 127);
  for (size_t i = 0; i < 128; i++){
    TEST(data[i + 1] == i);
  }
  for (size_t n = 0; n < 127; n++){
    TEST(packbits_encoder_put(n + 129, &encoder, data, &datasize) == 0);
    TEST(datasize == 0);
  }
  TEST(packbits_encoder_put(0, &encoder, data, &datasize) == 0);
  TEST(datasize == 129);
  TEST(data[0] == 127);
  for (size_t i = 0; i < 128; i++){
    TEST(data[i + 1] == i + 128);
  }
  TEST(packbits_encoder_put(1, &encoder, data, &datasize) == 0);
  TEST(datasize == 0);
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(datasize == 3);
  TEST(data[0] == 1);
  TEST(data[1] == 0);
  TEST(data[2] == 1);
}

static void test_packbits_encoder_put_error (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(packbits_encoder_put(1, &encoder, data, &datasize) == 1); //tried to put after finalized.
  TEST(packbits_errno == PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED);
}

static void test_packbits_encoder_finalize_error (){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t datasize;
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 0);
  TEST(packbits_encoder_finalize(&encoder, data, &datasize) == 1); //tried to finalize after finalized.
  TEST(packbits_errno == PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED);
}

void test_packbits_encoder (){
  testcase_packbits_encoder_put();
  testcase_packbits_encoder_put2();
  testcase_packbits_encoder_put3();
  testcase_packbits_encoder_put4();
  test_packbits_encoder_put_error();
  test_packbits_encoder_finalize_error();
}
