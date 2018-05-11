#include <asdf.hpp>
#include "gtest/gtest.h"

extern std::string test_data_path;

namespace Asdf {

TEST(ReaderTest, Tag)
{
    Asdf::AsdfFile asdf(test_data_path + std::string("test.asdf"));
    Asdf::Node tree = asdf.get_tree();
    EXPECT_EQ("tag:stsci.edu:asdf/core/asdf-1.1.0", tree.Tag());
}

} /* end namespace Asdf */
