
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "packbits-errno.h"
#include "packbits-encoder.h"
#include "packbits-decoder.h"

typedef enum mode {
  MODE_ENCODE,
  MODE_DECODE
} mode;

static int parse_args (int argc, char **argv, mode *modep, FILE **inputp, FILE **outputp, bool *showhelpp, bool *showversionp){
  mode mode = MODE_ENCODE;
  bool modegiven = false;
  char *inputfile = NULL;
  char *outputfile = NULL;
  bool showhelp = false;
  bool showversion = false;
  size_t index = 1;
  while (index < argc){
    if (strcmp(argv[index], "-o") == 0){
      if (outputfile == NULL){
        outputfile = argv[index +1];
        index += 2;
      }
      else {
        fprintf(stderr, "Already given output file \"%s\".", outputfile);
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-e") == 0){
      if (modegiven){
        fprintf(stderr, "Already given mode %d.", mode); 
        return 1;
      }
      else {
        modegiven = true;
        mode = MODE_ENCODE;
        index += 1;
      }
    }
    else 
    if (strcmp(argv[index], "-d") == 0){
      if (modegiven){
        fprintf(stderr, "Already given mode %d.", mode); 
        return 1;
      }
      else {
        modegiven = true;
        mode = MODE_DECODE;
        index += 1;
      }
    }
    else
    if (strcmp(argv[index], "-h") == 0){
      if (showhelp || showversion){
        fprintf(stderr, "Already given -h or -v option.");
        return 1;
      }
      else {
        showhelp = true;
        index += 1;
      }
    }
    else
    if (strcmp(argv[index], "-v") == 0){
      if (showhelp || showversion){
        fprintf(stderr, "Already given -h or -v option.");
        return 1;
      }
      else {
        showversion = true;
        index += 1;
      }
    }
    else {
      if (inputfile == NULL){
        inputfile = argv[index];
        index += 1;
      }
      else {
        fprintf(stderr, "Already given input file \"%s\".", inputfile);
        return 1;
      }
    }
  }
  if (showhelp){
    *inputp = stdin;
    *outputp = stdout;
    *modep = MODE_ENCODE;
    *showhelpp = true;
    *showversionp = false;
    return 0;
  }
  else 
  if (showversion){
    *inputp = stdin;
    *outputp = stdout;
    *modep = MODE_ENCODE;
    *showhelpp = false;
    *showversionp = true;
    return 0;
  }
  else {
    FILE *input;
    if (inputfile == NULL){
      input = stdin;
    }
    else {
      input = fopen(inputfile, "rb");
      if (input == NULL){
        fprintf(stderr, "Could not open input file \"%s\".", inputfile);
        return 1;
      }
    }
    FILE *output;
    if (outputfile == NULL){
      output = stdout;
    }
    else {
      output = fopen(outputfile, "wb");
      if (output == NULL){
        fprintf(stderr, "Could not open output file \"%s\".", outputfile);
        return 1;
      }
    }
    *modep = mode;
    *inputp = input;
    *outputp = output;
    *showhelpp = false;
    *showversionp = false;
    return 0;
  }
}

static int encode (FILE *input, FILE *output){
  packbits_encoder encoder;
  packbits_encoder_setup(&encoder);
  uint8_t compdata[PACKBITS_ENCODER_RESULT_BUFFER_SIZE];
  size_t compdatasize;
  while (true){
    int c = getc(input);
    if (c == EOF){
      if (ferror(input)){
        fprintf(stderr, "Could not read from input (errno = %d).\n", errno);
        return 1;
      }
      else {
        break;
      }
    }
    else {
      if (packbits_encoder_put(c, &encoder, compdata, &compdatasize)){
        char *message = packbits_errno_message(packbits_errno);
        fprintf(stderr, "Could not encode (packbits_errno = %d, %s).\n", packbits_errno, message);
        return 1;
      }
      if (0 < compdatasize){
        size_t writtensize = fwrite(compdata, compdatasize, 1, output);
        if (writtensize < 1){
          fprintf(stderr, "Could not write to output (errno = %d).\n", errno);
          return 1;
        }
      }
    }
  }
  if (packbits_encoder_finalize(&encoder, compdata, &compdatasize)){
    char *message = packbits_errno_message(packbits_errno);
    fprintf(stderr, "Could not encode (packbits_errno = %d, %s).\n", packbits_errno, message);
    return 1;
  }
  if (0 < compdatasize){
    size_t writtensize = fwrite(compdata, compdatasize, 1, output);
    if (writtensize < 1){
      fprintf(stderr, "Could not write to output (errno = %d).\n", errno);
      return 1;
    }
  }
  return 0;
}

static int decode (FILE *input, FILE *output){
  packbits_decoder decoder;
  packbits_decoder_setup(&decoder);
  uint8_t decompdata[PACKBITS_DECODER_RESULT_BUFFER_SIZE];
  size_t decompdatasize;
  while (true){
    int c = getc(input);
    if (c == EOF){
      if (ferror(input)){
        fprintf(stderr, "Could not read from input (errno = %d).\n", errno);
        return 1;
      }
      else {
        break;
      }
    }
    else {
      if (packbits_decoder_put(c, &decoder, decompdata, &decompdatasize)){
        char *message = packbits_errno_message(0);
        fprintf(stderr, "Could not decode (packbits_errno = %d, %s).\n", 0, message);
        return 1;
      }
      if (0 < decompdatasize){
        size_t writtensize = fwrite(decompdata, decompdatasize, 1, output);
        if (writtensize < 1){
          fprintf(stderr, "Could not write to output (errno = %d).\n", errno);
          return 1;
        }
      }
    }
  }
  if (packbits_decoder_finalize(&decoder, decompdata, &decompdatasize)){
    char *message = packbits_errno_message(0);
    fprintf(stderr, "Could not decode (packbits_errno = %d, %s).\n", 0, message);
    return 1;
  }
  if (0 < decompdatasize){
    size_t writtensize = fwrite(decompdata, decompdatasize, 1, output);
    if (writtensize < 1){
      fprintf(stderr, "Could not write to output (errno = %d).\n", errno);
      return 1;
    }
  }
  return 0;
}

const char HELP_MESSAGE[] = 
"Usage: packbits [OPTION]... [FILE]\n"
"Encode or decode FILE to PackBits format.\n"
"\n"
"OPTION:\n"
"  -o file: An output file. default is stdout.\n"
"  -e: Switch mode to encoding mode.\n"
"  -d: Switch mode to decoding mode.\n"
"  --help: Show help then exit.\n"
"  --version: Show version then exit.\n"
"FILE: An input file. default is stdin.\n"
;

const char VERSION_MESSAGE[] = 
"packbits 1.0.0\n"
;

int main (int argc, char **argv){
  mode mode;
  FILE *input;
  FILE *output;
  bool showhelp;
  bool showversion;
  if (parse_args(argc, argv, &mode, &input, &output, &showhelp, &showversion)){
    return 1;
  }
  if (showhelp){
    printf("%s", HELP_MESSAGE);
    return 0;
  }
  else 
  if (showversion){
    printf("%s", VERSION_MESSAGE);
    return 0;
  }
  else {
    switch (mode){
      case MODE_ENCODE: 
        return encode(input, output);
      case MODE_DECODE: 
        return decode(input, output);
      default: 
        return 1;
    }
  }
}
