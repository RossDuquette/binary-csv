#include <cstdlib>
#include <iostream>
#include <sstream>

#include "binary_csv_encoder.h"

BinaryCsvEncoder::BinaryCsvEncoder()
{
}

bool BinaryCsvEncoder::process(const char* input_filename,
                               const char* output_filename)
{
    in_file = std::ifstream(input_filename);
    out_file = std::ofstream(output_filename, std::ios::binary);
    bool success;
    if (!in_file.is_open() || !out_file.is_open()) {
        success = false;
    } else {
        success = encode_csv();
    }
    in_file.close();
    out_file.close();
    return success;
}

bool BinaryCsvEncoder::encode_csv()
{
    std::string line;
    while (std::getline(in_file, line)) {
        std::stringstream ss(line);
        std::string val;
        while (std::getline(ss, val, ',')) {
            if (val[0] == ' ') {
                val.erase(0, 1);
            }
            append_val(val);
        }
        append_val("\n");
    }
    return true;
}

void BinaryCsvEncoder::append_val(std::string val)
{
    if (is_int(val)) {
        append_int(val);
    } else if (is_float(val)) {
        append_float(val);
    } else {
        append_str(val);
    }
}

bool BinaryCsvEncoder::is_int(std::string val)
{
    if (!val.length()) {
        return false;
    }
    bool start = true;
    for (const char& chr : val) {
        if (start && is_sign(chr)) {
            start = false;
            continue;
        }
        start = false;
        if (!std::isdigit(chr)) {
            return false;
        }
    }
    return true;
}

bool BinaryCsvEncoder::is_float(std::string val)
{
    if (!val.length()) {
        return false;
    }
    bool start = true;
    bool found_decimal = false;
    found_exp = false;
    num_decimal_places = 0;
    num_sig_digits = 0;
    for (const char& chr : val) {
        if (start && is_sign(chr)) {
            start = false;
            continue;
        }
        start = false;
        if (!found_decimal && (chr == '.')) {
            found_decimal = true;
            continue;
        }
        if (!found_exp && ((chr == 'e') || (chr == 'E'))) {
            found_exp = true;
            // Don't allow decimals after exponent
            found_decimal = false;
            // Allow sign after exponent
            start = true;
            continue;
        }
        if (!std::isdigit(chr)) {
            return false;
        }
        if (found_decimal) {
            num_decimal_places++;
        }
        if (!found_exp) {
            num_sig_digits++;
        }
    }
    return true;
}

bool BinaryCsvEncoder::is_sign(const char chr)
{
    return (chr == '-') || (chr == '+');
}

void BinaryCsvEncoder::append_int(std::string val)
{
    long int num = strtol(val.c_str(), nullptr, 0);
    BinaryTypes type;
    if (num < 0) {
        type = TYPE_NEG_INT;
        num = -num;
    } else {
        type = TYPE_POS_INT;
    }
    if (num < 0x20) {
        append_header(type, false, num);
    } else {
        append_header(type, true, num);
        num >>= 5;
        while (num) {
            bool cont = num & 0xFFFFFF80;
            write_byte(((cont << 7) & 0x80)
                       | (num & 0x7F));
            num >>= 7;
        }
    }
}

void BinaryCsvEncoder::append_float(std::string val)
{
    double dnum = strtod(val.c_str(), nullptr);
    uint8_t header_data = ((found_exp << 4) & 0x10)
                        | (num_decimal_places & 0x0F);
    if (num_sig_digits < 7) {
        append_header(TYPE_FLOAT, false, header_data);
        float fnum = dnum;
        out_file.write((const char*) &fnum, sizeof(fnum));
    } else {
        append_header(TYPE_FLOAT, true, header_data);
        out_file.write((const char*) &dnum, sizeof(dnum));
    }
}

void BinaryCsvEncoder::append_str(std::string val)
{
    uint32_t str_len = val.length();
    if (str_len <= 0x3F) {
        append_header(TYPE_STRING, false, str_len);
    } else if (str_len <= 0x3FFF) {
        append_header(TYPE_STRING, true, str_len);
        write_byte((str_len >> 5) & 0xFF);
    } else {
        // Very long string, just write empty string to binary file.
        append_header(TYPE_STRING, false, 0);
        return;
    }
    out_file.write(val.c_str(), str_len);
}

void BinaryCsvEncoder::append_header(BinaryTypes type, bool cont_flag, uint8_t data)
{
    write_byte(((type << 6) & 0xC0)
               | ((cont_flag << 5) & 0x20)
               | (data & 0x1F));
}

void BinaryCsvEncoder::write_byte(uint8_t byte)
{
    out_file.write((const char*) &byte, sizeof(byte));
}
