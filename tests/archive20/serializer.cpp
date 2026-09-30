#include <doctest/doctest.h>
#include <igris/serialize/archive.h>
#include <stdexcept>

TEST_CASE("serialize_archive") 
{
	
}

TEST_CASE("serialize archive rejects buffer overflow")
{
    char byte = 0;
    igris::archive::binary_buffer_writer writer(&byte, sizeof(byte));
    CHECK_THROWS_AS(writer.dump(uint32_t{42}), std::out_of_range);

    igris::archive::binary_buffer_reader reader(&byte, sizeof(byte));
    uint32_t value = 0;
    CHECK_THROWS_AS(reader.load(value), std::out_of_range);
    CHECK_THROWS_AS(reader.skip(-1), std::out_of_range);

    std::string large(UINT16_MAX + 1u, 'x');
    CHECK_THROWS_AS(writer.dump(std::string_view(large)), std::length_error);
}

TEST_CASE("serialize archive consumes truncated fields")
{
    char storage[16] = {};
    igris::archive::binary_buffer_writer writer(storage, sizeof(storage));
    const char text[] = "abcd";
    writer.dump(igris::buffer((void *)text, 4));
    writer.dump(uint16_t{77});

    igris::archive::binary_buffer_reader reader(storage, sizeof(storage));
    char short_text[2] = {};
    reader.load(short_text, sizeof(short_text));
    uint16_t tail = 0;
    reader.load(tail);
    CHECK_EQ(short_text[0], 'a');
    CHECK_EQ(short_text[1], 'b');
    CHECK_EQ(tail, 77);
}
