#include <igris/util/signature.h>

class Signature0
{
public:
	int test();
};

class Signature1
{
public:
};

IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(has_test, test);

LT_BEGIN_TEST(igris_test_suite, signature)
{
	LT_CHECK(has_test<Signature0>() == true);
	LT_CHECK(has_test<Signature1>() == false);
}
LT_END_TEST(signature)