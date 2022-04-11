#ifndef BINARY_CSV_DECODER_H_
#define BINARY_CSV_DECODER_H_

#include <cstdint>
#include <fstream>
#include <string>

#include "binary_types.h"

class BinaryCsvDecoder {
    public:
        BinaryCsvDecoder();

        bool process(const char* input_filename,
                     const char* output_filename);

    private:
        bool decode_csv();
        int calc_in_file_length();
        void decode_byte(uint8_t byte);
        void decode_header(uint8_t type, uint8_t cont_flag, uint8_t data);
        void write_int();
        void write_float();
        void write_string();

        std::ifstream in_file;
        std::ofstream out_file;
        int bytes_read;
        int expected_len;
        bool exponent;
        int num_decimal_places;
        double dnum;
        float fnum;
        int inum;
        std::string str;
        enum {
            STATE_HEADER,
            STATE_POS_INT,
            STATE_NEG_INT,
            STATE_FLOAT,
            STATE_STRING_LENGTH,
            STATE_STRING,
        } state;
};

#endif
