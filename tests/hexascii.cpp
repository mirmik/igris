#include <doctest/doctest.h>
#include <igris/util/hexascii.h>

TEST_CASE("hexascii")
{
    const uint8_t arr[4] = {0xAB, 0xCD, 0xEF, 0x23};
    char res[8];
    uint8_t res2[4];

    hexascii_encode(arr, 4, res);

    CHECK_EQ(res[0], 'A');
    CHECK_EQ(res[1], 'B');
    CHECK_EQ(res[2], 'C');
    CHECK_EQ(res[3], 'D');
    CHECK_EQ(res[4], 'E');
    CHECK_EQ(res[5], 'F');
    CHECK_EQ(res[6], '2');
    CHECK_EQ(res[7], '3');

    hexascii_decode(res, 8, res2);

    CHECK_EQ(res2[0], 0xAB);
    CHECK_EQ(res2[1], 0xCD);
    CHECK_EQ(res2[2], 0xEF);
    CHECK_EQ(res2[3], 0x23);
}
