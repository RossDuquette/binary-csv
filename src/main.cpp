#include <cstdio>

#include "arg_parser.h"

int main(int argc, const char* argv[])
{
    ArgParser arg_parser;
    if (!arg_parser.parse(argc, argv)) {
        printf("%s\n", arg_parser.get_help());
        return 1;
    }
    printf("cmd: %d\ninfile: %s\noutfile: %s\ndelete: %d\n",
           arg_parser.get_arg_cmd(),
           arg_parser.get_input_filename(),
           arg_parser.get_output_filename(),
           arg_parser.delete_input_file());
    return 0;
}
