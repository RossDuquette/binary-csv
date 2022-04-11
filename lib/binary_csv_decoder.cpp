#include <cstring>

#include "binary_csv_decoder.h"

BinaryCsvDecoder::BinaryCsvDecoder()
{
}

bool BinaryCsvDecoder::process(const char* input_filename,
                               const char* output_filename)
{
    in_file = std::ifstream(input_filename, std::ios::binary);
    out_file = std::ofstream(output_filename);
    bool success;
    if (!in_file.is_open() || !out_file.is_open()) {
        success = false;
    } else {
        success = decode_csv();
    }
    in_file.close();
    out_file.close();
    return success;
}

bool BinaryCsvDecoder::decode_csv()
{
    state = STATE_HEADER;
    int in_file_len = calc_in_file_length();
    for (int i = 0; i < in_file_len; i++) {
        uint8_t byte;
        in_file.read((char*) &byte, sizeof(byte));
        decode_byte(byte);
    }
    return true;
}

int BinaryCsvDecoder::calc_in_file_length()
{
    in_file.seekg(0, in_file.end);
    int length = in_file.tellg();
    in_file.seekg(0, in_file.beg);
    return length;
}

void BinaryCsvDecoder::decode_byte(uint8_t byte)
{
    switch (state) {
    case STATE_HEADER:
        decode_header((byte >> 6) & 0x03,
                      (byte >> 5) & 0x01,
                      byte & 0x1F);
        break;
    case STATE_POS_INT:
        inum |= (byte & 0x7F) << (5 + 7 * bytes_read++);
        if (!(byte & 0x80)) {
            write_int();
            state = STATE_HEADER;
        }
        break;
    case STATE_NEG_INT:
        inum |= (byte & 0x7F) << (5 + 7 * bytes_read++);
        if (!(byte & 0x80)) {
            inum = -inum;
            write_int();
            state = STATE_HEADER;
        }
        break;
    case STATE_FLOAT:
        if (expected_len == sizeof(double)) {
            uint8_t* buf = (uint8_t*) &dnum;
            buf[bytes_read] = byte;
        } else {
            uint8_t* buf = (uint8_t*) &fnum;
            buf[bytes_read] = byte;
        }
        if (++bytes_read == expected_len) {
            write_float();
            state = STATE_HEADER;
        }
        break;
    case STATE_STRING_LENGTH:
        expected_len |= (byte << 5);
        state = STATE_STRING;
        break;
    case STATE_STRING:
        str.push_back((char) byte);
        if (++bytes_read == expected_len) {
            write_string();
            state = STATE_HEADER;
        }
        break;
    }
}

void BinaryCsvDecoder::decode_header(uint8_t type, uint8_t cont_flag, uint8_t data)
{
    bytes_read = 0;
    switch (type) {
    case TYPE_POS_INT:
        inum = data;
        if (cont_flag) {
            state = STATE_POS_INT;
        } else {
            write_int();
        }
        break;
    case TYPE_NEG_INT:
        inum = data;
        if (cont_flag) {
            state = STATE_NEG_INT;
        } else {
            inum = -inum;
            write_int();
        }
        break;
    case TYPE_FLOAT:
        if (cont_flag) {
            dnum = 0;
            expected_len = sizeof(double);
        } else {
            fnum = 0;
            expected_len = sizeof(float);
        }
        exponent = (data >> 4) & 0x01;
        num_decimal_places = data & 0x0F;
        state = STATE_FLOAT;
        break;
    case TYPE_STRING:
        str = "";
        expected_len = data;
        if (expected_len) {
            if (cont_flag) {
                state = STATE_STRING_LENGTH;
            } else {
                state = STATE_STRING;
            }
        } else {
            write_string();
        }
        break;
    }
}

void BinaryCsvDecoder::write_int()
{
    char cstr[64];
    sprintf(cstr, "%d,", inum);
    out_file.write(cstr, strlen(cstr));
}

void BinaryCsvDecoder::write_float()
{
    char cstr[128];
    if (expected_len == sizeof(float)) {
        dnum = fnum;
    }
    if (exponent) {
        sprintf(cstr, "%.*E,", num_decimal_places, dnum);
    } else {
        sprintf(cstr, "%.*f,", num_decimal_places, dnum);
    }
    out_file.write(cstr, strlen(cstr));
}

void BinaryCsvDecoder::write_string()
{
    if (!((str.length() == 1) && (str[0] == '\n'))) {
        str.append(",");
    }
    out_file.write(str.c_str(), str.length());
}
