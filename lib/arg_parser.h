#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

enum ArgCmd {
    ARG_CMD_INVALID,
    ARG_CMD_DECODE,
    ARG_CMD_ENCODE,
};

class ArgParser {
    public:
        ArgParser();

        const char* get_help() const;

        bool parse(int argc, const char** argv);

        ArgCmd get_arg_cmd() const;
        const char* get_input_filename() const;
        const char* get_output_filename() const;
        bool delete_input_file() const;

    private:
        bool is_arg_equal(const char* arg,
                          const char* short_option,
                          const char* long_option) const;
        void set_output_filename();
        void strip_extension(char* fname);

        ArgCmd arg_cmd;
        const char* input_filename;
        const char* output_filename;
        bool keep_input_file;
};

#endif
