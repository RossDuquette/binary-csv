#include <cstdio>
#include <cstring>

#include "arg_parser.h"

ArgParser::ArgParser() :
    arg_cmd(ARG_CMD_INVALID),
    input_filename(nullptr),
    output_filename(nullptr),
    keep_input_file(false)
{
}

const char* ArgParser::get_help() const
{
    return "\
Usage: binary_csv [options] infile [-o outfile] \n\
  -d, --decode          decode the BCSV file to CSV \n\
  -e, --encode          encode the CSV file to BCSV \n\
  -k, --keep-infile     do not delete the infile \n\
  -h, --help            display this help text \n\
  -o, --outfile         specify the name of output file \n\
";
}

bool ArgParser::parse(int argc, const char** argv)
{
    for (int i = 1; i < argc; i++) {
        if (is_arg_equal(argv[i], "-d", "--decode")) {
            arg_cmd = ARG_CMD_DECODE;
            if ((i + 1) < argc) {
                i++;
                input_filename = argv[i];
            } else {
                return false;
            }
        } else if (is_arg_equal(argv[i], "-e", "--encode")) {
            arg_cmd = ARG_CMD_ENCODE;
            if ((i + 1) < argc) {
                i++;
                input_filename = argv[i];
            } else {
                return false;
            }
        } else if (is_arg_equal(argv[i], "-k", "--keep-infile")) {
            keep_input_file = true;
        } else if (is_arg_equal(argv[i], "-o", "--outfile")) {
            if ((i + 1) < argc) {
                i++;
                output_filename = argv[i];
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    if ((arg_cmd != ARG_CMD_INVALID) && input_filename) {
        if (!output_filename) {
            set_output_filename();
        }
        return true;
    }
    return false;
}

bool ArgParser::is_arg_equal(const char* arg,
                             const char* short_option,
                             const char* long_option) const
{
    return (strcmp(arg, short_option) == 0)
        || (strcmp(arg, long_option) == 0);
}

void ArgParser::set_output_filename()
{
    static char filename[256];
    snprintf(filename, sizeof(filename), "%s", input_filename);
    char* extension = strrchr(filename, '.');
    if (!extension) {
        if (arg_cmd == ARG_CMD_ENCODE) {
            strcat(filename, ".bcsv");
        } else {
            strcat(filename, ".csv");
        }
    } else {
        if (arg_cmd == ARG_CMD_ENCODE) {
            strcpy(extension, ".bcsv");
        } else {
            strcpy(extension, ".csv");
        }
    }
    output_filename = filename;
}

ArgCmd ArgParser::get_arg_cmd() const
{
    return arg_cmd;
}

const char* ArgParser::get_input_filename() const
{
    return input_filename;
}

const char* ArgParser::get_output_filename() const
{
    return output_filename;
}

bool ArgParser::delete_input_file() const
{
    return !keep_input_file;
}
