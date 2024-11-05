
#include <threads.h>
#include "packbits-errno.h"

thread_local _packbits_errno packbits_errno = PACKBITS_ERRNO_NONE;

char __stdcall *packbits_errno_message (_packbits_errno err){
  switch (err){
    case PACKBITS_ERRNO_NONE:
      return "Funcall was successed.";
    case PACKBITS_ERRNO_ENCODER_ALREADY_FINALIZED:
      return "packbits_encoder is already finzlied by packbits_encoder_finalize().";
    case PACKBITS_ERRNO_ENCODER_ILLIGAL_STATE:
      return "packbits_encoder has illigal state.";
    case PACKBITS_ERRNO_DECODER_ALREADY_FINALIZED:
      return "packbits_decoder is already finzlied by packbits_decoder_finalize().";
    case PACKBITS_ERRNO_DECODER_ILLIGAL_STATE:
      return "packbits_decoder has illigal state.";
    case PACKBITS_ERRNO_DECODER_INCOMPLETE_STATE:
      return "packbits_decoder has incomplete state.";
    case PACKBITS_ERRNO_DECODER_INVALID_INPUT:
      return "packbits_decoder given an invalid input.";
    case PACKBITS_ERRNO_INCOMPLETE_INPUT:
      return "Function given an incomplete input.";
    case PACKBITS_ERRNO_FAILED_MEMORY_ALLOCATION:
      return "Function could not allocate memories.";
    default:
      return "Undefined errno.";
  }
}
