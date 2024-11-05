
#include <stdint.h>
#include <stddef.h>
#include "packbits-errno.h"
#include "packbits-decoder.h"

inline void __stdcall packbits_decoder_setup (packbits_decoder *decoder){
  decoder->state = PACKBITS_DECODER_PENDING;
  decoder->mode = PACKBITS_DECODER_UNDETERMINED;
  decoder->repeat = 0;
}

inline int __stdcall packbits_decoder_put (uint8_t byte, packbits_decoder *decoder, uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  switch (decoder->state){
    case PACKBITS_DECODER_PENDING:
      switch (decoder->mode){
        case PACKBITS_DECODER_UNDETERMINED:
          if (0 <= ((int8_t)byte)){
            decoder->mode = PACKBITS_DECODER_DISCRETE;
            decoder->repeat = byte +1;
            *datasizep = 0;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
          else 
          if (-128 == ((int8_t)byte)){
            decoder->mode = PACKBITS_DECODER_UNDETERMINED;
            decoder->repeat = 0;
            *datasizep = 0;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
          else {
            decoder->mode = PACKBITS_DECODER_REPEATING;
            decoder->repeat = 1- ((int8_t)byte);
            *datasizep = 0;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
        case PACKBITS_DECODER_REPEATING:
          for (size_t i = 0; i < decoder->repeat; i++){
            data[i] = byte;
          }
          *datasizep = decoder->repeat;
          decoder->mode = PACKBITS_DECODER_UNDETERMINED;
          packbits_errno = PACKBITS_ERRNO_NONE;
          return 0;
        case PACKBITS_DECODER_DISCRETE:
          data[0] = byte;
          *datasizep = 1;
          decoder->repeat--;
          if (decoder->repeat == 0){
            decoder->mode = PACKBITS_DECODER_UNDETERMINED;
          }
          packbits_errno = PACKBITS_ERRNO_NONE;
          return 0;
        default:
          packbits_errno = PACKBITS_ERRNO_DECODER_ILLIGAL_STATE; //unknown encoder->mode.
          return 1;
      }
    case PACKBITS_DECODER_FINALIZED:
      packbits_errno = PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED;
      return 1;
    default:
      packbits_errno = PACKBITS_ERRNO_DECODER_ILLIGAL_STATE; //unknown encoder->state.
      return 1;
  }
}

inline int __stdcall packbits_decoder_finalize (packbits_decoder *decoder, uint8_t data[PACKBITS_DECODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  switch (decoder->state){
    case PACKBITS_DECODER_PENDING:
      switch (decoder->mode){
        case PACKBITS_DECODER_UNDETERMINED:
          decoder->state = PACKBITS_DECODER_FINALIZED;
          *datasizep = 0;
          packbits_errno = PACKBITS_ERRNO_NONE;
          return 0;
        case PACKBITS_DECODER_REPEATING:
          packbits_errno = PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE;
          return 1;
        case PACKBITS_DECODER_DISCRETE:
          packbits_errno = PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE;
          return 1;
        default: 
          packbits_errno = PACKBITS_ERRNO_DECODER_ILLIGAL_STATE; //unknown encoder->mode.
          return 1;
      }
    case PACKBITS_DECODER_FINALIZED:
      packbits_errno = PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED;
      return 1;
    default: 
      packbits_errno = PACKBITS_ERRNO_DECODER_ILLIGAL_STATE; //unknown encoder->state.
      return 1;
  }
}
