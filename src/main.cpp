#include <cstdio>

#include "arg_parser.h"
#include "binary_csv_decoder.h"
#include "binary_csv_encoder.h"

int main(int argc, const char* argv[])
{
    ArgParser arg_parser;
    if (!arg_parser.parse(argc, argv)) {
        printf("%s\n", arg_parser.get_help());
        return 1;
    }

    bool success = false;
    if (arg_parser.get_arg_cmd() == ARG_CMD_ENCODE) {
        BinaryCsvEncoder encoder;
        success = encoder.process(arg_parser.get_input_filename(),
                                  arg_parser.get_output_filename());
    } else if (arg_parser.get_arg_cmd() == ARG_CMD_DECODE) {
        BinaryCsvDecoder decoder;
        success = decoder.process(arg_parser.get_input_filename(),
                                  arg_parser.get_output_filename());
    }
    if (success && arg_parser.delete_input_file()) {
        std::remove(arg_parser.get_input_filename());
    }
    return 0;
}
