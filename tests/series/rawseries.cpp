#include <doctest/doctest.h>
#include <igris/series/series.h>

struct A
{
    int a;
    double b;
    int c;
} __attribute__((packed));

TEST_CASE("Series")
{
    igris::series series;

    series.set_elemsize(sizeof(A));
    series.annotator().add<int>("a");
    series.annotator().add<double>("b");
    series.annotator().add<int>("c");

    A obj{2, 3, 4};
    series.push_object(&obj, sizeof(A));

    CHECK_EQ((*series.annotations())[0].offset, 0);
    CHECK_EQ((*series.annotations())[1].offset, 4);
    CHECK_EQ((*series.annotations())[2].offset, 12);
}