#include "unity_groups.h"

void setUp()
{
}

void tearDown()
{
}

int main()
{
    UNITY_BEGIN();
    TEST_RUN_GROUP(decoder);
    TEST_RUN_GROUP(encoder);
    return UNITY_END();
}
