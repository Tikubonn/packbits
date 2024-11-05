
#include <stdint.h>
#include <stddef.h>
#include "packbits-errno.h"
#include "packbits-encoder.h"

inline void __stdcall packbits_encoder_setup (packbits_encoder *encoder){
  encoder->state = PACKBITS_ENCODER_PENDING;
  encoder->mode = PACKBITS_ENCODER_UNDETERMINED;
  encoder->seek = 0;
}

static inline void copy_to_repeating (packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  data[0] = 1 - encoder->seek;
  data[1] = encoder->data[0];
  *datasizep = 2;
}

static inline void copy_to_discrete (packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  data[0] = encoder->seek -1;
  for (size_t i = 0; i < encoder->seek; i++){
    data[i + 1] = encoder->data[i];
  }
  *datasizep = encoder->seek +1;
}

inline int __stdcall packbits_encoder_put (uint8_t byte, packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  switch (encoder->state){
    case PACKBITS_ENCODER_PENDING:
      switch (encoder->mode){
        case PACKBITS_ENCODER_UNDETERMINED:
          if (encoder->seek < 2){
            encoder->data[encoder->seek] = byte;
            encoder->seek++;
            if (encoder->seek == 2){
              if (encoder->data[0] == encoder->data[1]){
                encoder->mode = PACKBITS_ENCODER_REPEATING;
              }
              else {
                encoder->mode = PACKBITS_ENCODER_DISCRETE;
              }
            }
            *datasizep = 0;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
          else {
            packbits_errno = PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE; //if encoder->mode == PACKBITS_ENCODER_UNDETERMINED, encoder->seek must be n < 2.
            return 1;
          }
        case PACKBITS_ENCODER_REPEATING:
          if (encoder->seek < 128){
            if (encoder->data[0] == byte){
              encoder->seek++;
              *datasizep = 0;
              packbits_errno = PACKBITS_ERRNO_NONE; 
              return 0;
            }
            else {
              copy_to_repeating(encoder, data, datasizep);
              encoder->mode = PACKBITS_ENCODER_UNDETERMINED;
              encoder->data[0] = byte;
              encoder->seek = 1;
              packbits_errno = PACKBITS_ERRNO_NONE; 
              return 0;
            }
          }
          else {
            copy_to_repeating(encoder, data, datasizep);
            encoder->mode = PACKBITS_ENCODER_UNDETERMINED;
            encoder->data[0] = byte;
            encoder->seek = 1;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
        case PACKBITS_ENCODER_DISCRETE:
          if (encoder->seek < 128){
            if (encoder->data[encoder->seek -1] != byte){
              encoder->data[encoder->seek] = byte;
              encoder->seek++;
              *datasizep = 0;
              packbits_errno = PACKBITS_ERRNO_NONE;
              return 0;
            }
            else {
              copy_to_discrete(encoder, data, datasizep);
              encoder->mode = PACKBITS_ENCODER_UNDETERMINED;
              encoder->data[0] = byte;
              encoder->seek = 1;
              packbits_errno = PACKBITS_ERRNO_NONE;
              return 0;
            }
          }
          else {
            copy_to_discrete(encoder, data, datasizep);
            encoder->mode = PACKBITS_ENCODER_UNDETERMINED;
            encoder->data[0] = byte;
            encoder->seek = 1;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
        default:
          packbits_errno = PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE; //unknown encoder->mode.
          return 1;
      }
    case PACKBITS_ENCODER_FINALIZED:
      packbits_errno = PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED;
      return 1;
    default:
      packbits_errno = PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE; //unknown encoder->state.
      return 1;
  }
}

inline int __stdcall packbits_encoder_finalize (packbits_encoder *encoder, uint8_t data[PACKBITS_ENCODER_RESULT_BUFFER_SIZE], size_t *datasizep){
  switch (encoder->state){
    case PACKBITS_ENCODER_PENDING:
      switch (encoder->mode){
        case PACKBITS_ENCODER_UNDETERMINED:
          if (0 < encoder->seek){
            copy_to_discrete(encoder, data, datasizep);
            encoder->state = PACKBITS_ENCODER_FINALIZED;
            encoder->mode = PACKBITS_ENCODER_PENDING;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
          else {
            *datasizep = 0;
            encoder->state = PACKBITS_ENCODER_FINALIZED;
            encoder->mode = PACKBITS_ENCODER_PENDING;
            packbits_errno = PACKBITS_ERRNO_NONE;
            return 0;
          }
        case PACKBITS_ENCODER_REPEATING:
          copy_to_repeating(encoder, data, datasizep);
          encoder->state = PACKBITS_ENCODER_FINALIZED;
          encoder->mode = PACKBITS_ENCODER_PENDING;
          packbits_errno = PACKBITS_ERRNO_NONE;
          return 0;
        case PACKBITS_ENCODER_DISCRETE:
          copy_to_discrete(encoder, data, datasizep);
          encoder->state = PACKBITS_ENCODER_FINALIZED;
          encoder->mode = PACKBITS_ENCODER_PENDING;
          packbits_errno = PACKBITS_ERRNO_NONE;
          return 0;
        default:
          packbits_errno = PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE; //unknown encoder->mode.
          return 1;
      }
    case PACKBITS_ENCODER_FINALIZED:
      packbits_errno = PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED;
      return 1;
    default:
      packbits_errno = PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE; //unknown encoder->state.
      return 1;
  }
}
