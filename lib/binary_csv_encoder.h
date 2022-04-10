#ifndef BINARY_CSV_ENCODER_H_
#define BINARY_CSV_ENCODER_H_

#include <cstdint>
#include <fstream>
#include <string>

#include "binary_types.h"

class BinaryCsvEncoder {
    public:
        BinaryCsvEncoder();

        bool process(const char* input_filename,
                     const char* output_filename);

    private:
        bool encode_csv();
        void append_val(std::string val);
        bool is_int(std::string val);
        bool is_float(std::string val);
        bool is_sign(const char chr);
        void append_int(std::string val);
        void append_float(std::string val);
        void append_str(std::string val);
        void append_header(BinaryTypes type, bool cont_flag, uint8_t data);
        void write_byte(uint8_t byte);

        std::ifstream in_file;
        std::ofstream out_file;
        bool found_exp;
        uint8_t num_decimal_places;
        uint8_t num_sig_digits;
};

#endif
