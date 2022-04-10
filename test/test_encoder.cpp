#include <cstdio>

#include "binary_csv_encoder.h"
#include "unity_groups.h"

static BinaryCsvEncoder* encoder = nullptr;
static const char* infile = "./test/example.csv";
static const char* outfile = "./test/out.bcsv";
static const char* expected_outfile = "./test/example.bcsv";

TEST_SETUP(encoder)
{
    encoder = new BinaryCsvEncoder;
}

TEST_TEARDOWN(encoder)
{
    std::remove(outfile);
    delete encoder;
}


TEST(encoder, example)
{
    TEST_ASSERT_EQUAL(true, encoder->process(infile, outfile));
    char cmd[128];
    sprintf(cmd, "diff %s %s", outfile, expected_outfile);
    TEST_ASSERT_EQUAL(0, system(cmd));
}


TEST_GROUP_RUNNER(encoder)
{
    TEST_RUN(encoder, example);
}
