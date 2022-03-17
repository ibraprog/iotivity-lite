#include "ausy_encoder.h"
#include "deps/json-parser/json.h"
#include "oc_log.h"
#include "deps/tinycbor/src/cborjson.h"

static CborError ausy_encode_value_2_cbor(json_value* value, CborEncoder* currentEncoder);

static CborError ausy_encode_map_2_cbor(json_value* value, CborEncoder* currentEncoder)
{
    CborError err = CborNoError;
    int length, x;
    if (value == NULL) {
        return err;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        const char* key = value->u.object.values[x].name;
        err |= cbor_encode_text_stringz(currentEncoder, key);
        err |= ausy_encode_value_2_cbor(value->u.object.values[x].value, currentEncoder);
    }

    return err;
}

static CborError ausy_encode_array_2_cbor(json_value* value, CborEncoder* currentEncoder)
{
    CborError err = CborNoError;
    int length, x;
    if (value == NULL) {
        return err;
    }
    length = value->u.array.length;
    for (x = 0; x < length; x++) {
        err |= ausy_encode_value_2_cbor(value->u.array.values[x], currentEncoder);
    }

    return err;
}

static CborError ausy_encode_value_2_cbor(json_value* value, CborEncoder* currentEncoder)
{
    CborError err = CborNoError;

    if (value == NULL) {
        return err;
    }

    switch (value->type) {
        case json_none:
        {
            err |= CborErrorUnsupportedType;
            break;
        }
        case json_null:
        {
            err |= cbor_encode_null(currentEncoder);
            break;
        }
        case json_object:
        {
            size_t size = (size_t) (value->u.object.length);
            CborEncoder mapEncoder;
            err |= cbor_encoder_create_map(currentEncoder, &mapEncoder, size);
            err |= ausy_encode_map_2_cbor(value, &mapEncoder);
            cbor_encoder_close_container(currentEncoder, &mapEncoder);
            break;
        }
        case json_array:
        {
            size_t size = (size_t) (value->u.array.length);
            CborEncoder arrayEncoder;
            err |= cbor_encoder_create_array(currentEncoder, &arrayEncoder, size);
            err |= ausy_encode_array_2_cbor(value, &arrayEncoder);
            cbor_encoder_close_container(currentEncoder, &arrayEncoder);
            break;
        }
        case json_integer:
        {
            err |= cbor_encode_int(currentEncoder, (int64_t)value->u.integer);
            break;
        }
        case json_double:
        {
            err |= cbor_encode_double(currentEncoder, value->u.dbl);
            break;
        }
        case json_string:
        {
            err|= cbor_encode_text_stringz(currentEncoder, value->u.string.ptr);
            break;
        }
        case json_boolean:
        {
            bool val = value->u.boolean ? true : false;
            err |= cbor_encode_boolean(currentEncoder, val);
            break;
        }
    }

    return err;
}

bool ausy_encode_payload_2_cbor(uint8_t* payload,
                                const size_t payload_len,
                                const oc_content_format_t cf)
{
    bool success = false;
    if (cf == TEXT_PLAIN)
    {
        char* const payload_string = (char*) payload;
        json_value* json_payload = json_parse(payload_string, payload_len);
        if (json_payload)
        {
            OC_DBG("Payload parsed successfully!\n");
            CborEncoder encoder;
            cbor_encoder_init(&encoder, payload, payload_len, 0);
            CborError err = ausy_encode_value_2_cbor(json_payload, &encoder);
            success = err == CborNoError;
        }
        else
        {
            OC_ERR("ausyencoder: error parsing json payload!\n");
        }
        json_value_free(json_payload);
    }
    return success;
}

bool ausy_decode_response_from_cbor_2_text(uint8_t* buffer,
                                           uint32_t buffer_size)
{
    FILE* out = NULL;
    CborValue cbor_value;
    CborParser cbor_parser;
    CborError err = cbor_parser_init(buffer, (size_t)buffer_size, 0, &cbor_parser, &cbor_value);
    out = fopen("data.json", "w+");
    if (!out) {
        OC_ERR("data.json failed to open!\n");
        return false;
    } else {
        //OC_DBG("data.json opened suscessfully!\n");
        err |= cbor_value_to_json(out, &cbor_value, 0);
        if (err == CborNoError) {
            // Move the file pointer to the start.
            fseek(out, 0, SEEK_SET);
            /*char ch;
            int i = 0;
            do {
                ch = fgetc(out);
                if (ch == 39) {
                    buffer[i] = 92;
                    buffer[i+1] = 92;
                    i += 2;
                }
                buffer[i] = ch;
                i++;
            } while (ch != EOF);*/
            if (fgets(buffer,(int)buffer_size, out) != NULL) {
             //OC_DBG("Decoded data = %s\n", str);
                fclose(out);
                return true;
            } else {
             OC_ERR("fgets return null ptr!\n");
             fclose(out);
             return false;
            }
        } else {
            OC_ERR("Error when decoding the cbor data!\n");
            fclose(out);
            return false;
        }
    }
}