
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <packbits/packbits.h>
#include "test.h"

static void testcase_packbits_encode (){
  char plaindata[] = {'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'd', 'e', 'f', 'g'};
  size_t encodeddatasize;
  int8_t *encodeddata = packbits_encode(plaindata, sizeof(plaindata), PACKBITS_DEFAULT_REALLOC_FUNC, &encodeddatasize);
  TEST(encodeddata != NULL);
  TEST(encodeddatasize == 11);
  TEST(encodeddata[0] == -1);
  TEST(encodeddata[1] == 'a');
  TEST(encodeddata[2] == -2);
  TEST(encodeddata[3] == 'b');
  TEST(encodeddata[4] == -3);
  TEST(encodeddata[5] == 'c');
  TEST(encodeddata[6] == 3);
  TEST(encodeddata[7] == 'd');
  TEST(encodeddata[8] == 'e');
  TEST(encodeddata[9] == 'f');
  TEST(encodeddata[10] == 'g');
  free(encodeddata); 
}

static void testcase_packbits_encode2 (){
  char plaindata[258];
  for (size_t i = 0; i < sizeof(plaindata); i++){
    plaindata[i] = 'a';
  }
  size_t encodeddatasize;
  int8_t *encodeddata = packbits_encode(plaindata, sizeof(plaindata), PACKBITS_DEFAULT_REALLOC_FUNC, &encodeddatasize);
  TEST(encodeddata != NULL);
  TEST(encodeddatasize == 6);
  TEST(encodeddata[0] == -127);
  TEST(encodeddata[1] == 'a');
  TEST(encodeddata[2] == -127);
  TEST(encodeddata[3] == 'a');
  TEST(encodeddata[4] == -1);
  TEST(encodeddata[5] == 'a');
  free(encodeddata);
}

static void testcase_packbits_encode3 (){
  char plaindata[258];
  for (size_t i = 0; i < sizeof(plaindata); i++){
    plaindata[i] = i & 0xff;
  }
  size_t encodeddatasize;
  int8_t *encodeddata = packbits_encode(plaindata, sizeof(plaindata), PACKBITS_DEFAULT_REALLOC_FUNC, &encodeddatasize);
  TEST(encodeddata != NULL);
  TEST(encodeddatasize == 261);
  TEST(encodeddata[0] == 127);
  for (size_t i = 0; i < 128; i++){
    TEST(encodeddata[i + 1] == plaindata[i]);
  }
  TEST(encodeddata[129] == 127);
  for (size_t i = 0; i < 128; i++){
    TEST(encodeddata[i + 130] == plaindata[i + 128]);
  }
  TEST(encodeddata[258] == 1);
  TEST(encodeddata[259] == 0);
  TEST(encodeddata[260] == 1);
  free(encodeddata);
}

static void testcase_packbits_encode4 (){
  char plaindata[] = {'H', 'e', 'l', 'l', 'o', '.'};
  size_t encodeddatasize;
  int8_t *encodeddata = packbits_encode(plaindata, sizeof(plaindata), PACKBITS_DEFAULT_REALLOC_FUNC, &encodeddatasize);
  TEST(encodeddata != NULL);
  TEST(encodeddatasize == 8);
  TEST(encodeddata[0] == 1);
  TEST(encodeddata[1] == 'H');
  TEST(encodeddata[2] == 'e');
  TEST(encodeddata[3] == -1);
  TEST(encodeddata[4] == 'l');
  TEST(encodeddata[5] == 1);
  TEST(encodeddata[6] == 'o');
  TEST(encodeddata[7] == '.');
  free(encodeddata); 
}

static void testcase_packbits_decode (){
  char encodeddata[] = {-1, 'a', -2, 'b', -3, 'c', 3, 'd', 'e', 'f', 'g'};
  size_t decodeddatasize;
  int8_t *decodeddata = packbits_decode(encodeddata, sizeof(encodeddata), PACKBITS_DEFAULT_REALLOC_FUNC, &decodeddatasize);
  TEST(decodeddatasize == 13);
  TEST(decodeddata != NULL);
  TEST(decodeddata[0] == 'a'); 
  TEST(decodeddata[1] == 'a'); 
  TEST(decodeddata[2] == 'b'); 
  TEST(decodeddata[3] == 'b'); 
  TEST(decodeddata[4] == 'b'); 
  TEST(decodeddata[5] == 'c'); 
  TEST(decodeddata[6] == 'c'); 
  TEST(decodeddata[7] == 'c'); 
  TEST(decodeddata[8] == 'c'); 
  TEST(decodeddata[9] == 'd'); 
  TEST(decodeddata[10] == 'e'); 
  TEST(decodeddata[11] == 'f'); 
  TEST(decodeddata[12] == 'g');
  free(decodeddata);
}

static void testcase_packbits_decode2 (){
  char encodeddata[] = {-127, 'a', -127, 'a', -1, 'a'};
  size_t decodeddatasize;
  int8_t *decodeddata = packbits_decode(encodeddata, sizeof(encodeddata), PACKBITS_DEFAULT_REALLOC_FUNC, &decodeddatasize);
  TEST(decodeddatasize == 258);
  TEST(decodeddata != NULL);
  for (size_t i = 0; i < 258; i++){
    TEST(decodeddata[i] == 'a');
  }
  free(decodeddata);
}

static void testcase_packbits_decode3 (){
  char encodeddata[261];
  encodeddata[0] = 127;
  for (size_t i = 0; i < 128; i++){
    encodeddata[i + 1] = i;
  }
  encodeddata[129] = 127;
  for (size_t i = 0; i < 128; i++){
    encodeddata[i + 130] = i + 128;
  }
  encodeddata[258] = 1;
  encodeddata[259] = 0;
  encodeddata[260] = 1;
  size_t decodeddatasize;
  int8_t *decodeddata = packbits_decode(encodeddata, sizeof(encodeddata), PACKBITS_DEFAULT_REALLOC_FUNC, &decodeddatasize);
  TEST(decodeddatasize == 258);
  TEST(decodeddata != NULL);
  for (size_t i = 0; i < 258; i++){
    TEST(decodeddata[i] == (int8_t)(i & 0xff));
  }
  free(decodeddata);
}

static void testcase_packbits_decode4 (){
  char encodeddata[] = {1, 'H', 'e', -1, 'l', 1, 'o', '.'};
  size_t decodeddatasize;
  int8_t *decodeddata = packbits_decode(encodeddata, sizeof(encodeddata), PACKBITS_DEFAULT_REALLOC_FUNC, &decodeddatasize);
  TEST(decodeddatasize == 6);
  TEST(decodeddata != NULL);
  TEST(decodeddata[0] == 'H'); 
  TEST(decodeddata[1] == 'e'); 
  TEST(decodeddata[2] == 'l'); 
  TEST(decodeddata[3] == 'l'); 
  TEST(decodeddata[4] == 'o'); 
  TEST(decodeddata[5] == '.'); 
  free(decodeddata);
}

void test_packbits (){
  testcase_packbits_encode();
  testcase_packbits_encode2();
  testcase_packbits_encode3();
  testcase_packbits_encode4();
  testcase_packbits_decode();
  testcase_packbits_decode2();
  testcase_packbits_decode3();
  testcase_packbits_decode4();
}
