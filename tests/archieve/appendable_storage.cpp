#include <doctest/doctest.h>
#include <igris/serialize/serialize_storage.h>

using namespace std::string_literals;

TEST_CASE("appendable_storage") 
{
	auto storage = igris::appendable_storage<std::string>();

	storage.dumps("HelloWorld");
	std::string str = storage.loads(storage.avail());

	CHECK_EQ(str, "HelloWorld"s);
}

TEST_CASE("appendable_storage ctr") 
{
	auto in = "HelloWorld"s;
	auto storage = igris::appendable_storage(in);

	std::string str = storage.loads(storage.avail());

	CHECK_EQ(storage.avail(), 10);
	CHECK_EQ(str, "HelloWorld"s);
}