#include <string>
#include <sstream>

#include <asdf-cpp/asdf.hpp>

#include "gtest/gtest.h"

extern std::string test_data_path;
using namespace Asdf;

TEST(ReaderTest, Tag)
{
    Asdf::AsdfFile asdf(test_data_path + std::string("test.asdf"));
    Asdf::Node tree = asdf.get_tree();
    EXPECT_EQ("tag:stsci.edu:asdf/core/asdf-1.1.0", tree.Tag());
}

TEST(ReaderTest, FromStream)
{
    AsdfFile asdf;
    Node tree = asdf.get_tree();
    tree["top"] = "hello";

    std::vector<int> nums;
    for (int i = 0; i < 100; i++)
    {
        nums.push_back(i);
    }

    tree["nums"] = asdf.create_data_block<int>(nums.data(), nums.size());

    std::stringstream asdf_stream;
    asdf_stream << asdf;

    AsdfFile new_asdf(asdf_stream);
    Node new_tree = new_asdf.get_tree();
    EXPECT_EQ(new_tree["top"].as<std::string>(), "hello");

    auto num_array = new_asdf.get_array<int>(new_tree["nums"]);
    int *data = num_array.get_raw_data();
    for (int i = 0; i < 100; i++)
    {
        EXPECT_EQ(data[i], i);
    }
}
