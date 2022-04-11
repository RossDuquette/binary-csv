#include <cstdio>

#include "binary_csv_decoder.h"
#include "unity_groups.h"

static BinaryCsvDecoder* decoder = nullptr;
static const char* infile = "./test/example.bcsv";
static const char* outfile = "./test/out.csv";
static const char* expected_outfile = "./test/example.csv";

TEST_SETUP(decoder)
{
    decoder = new BinaryCsvDecoder;
}

TEST_TEARDOWN(decoder)
{
    std::remove(outfile);
    delete decoder;
}


TEST(decoder, example)
{
    TEST_ASSERT_EQUAL(true, decoder->process(infile, outfile));
    char cmd[128];
    sprintf(cmd, "diff %s %s", outfile, expected_outfile);
    TEST_ASSERT_EQUAL(0, system(cmd));
}


TEST_GROUP_RUNNER(decoder)
{
    TEST_RUN(decoder, example);
}
