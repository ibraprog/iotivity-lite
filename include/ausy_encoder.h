#ifndef AUSY_ENCODER_H
#define AUSY_ENCODER_H

#include "oc_ri.h"

bool ausy_encode_payload_2_cbor(uint8_t* payload,
                                const size_t payload_len,
                                const oc_content_format_t cf);

bool ausy_decode_response_from_cbor_2_text(uint8_t* data_cbor,
                                           uint32_t data_size);

#endif // AUSY_ENCODER_H

