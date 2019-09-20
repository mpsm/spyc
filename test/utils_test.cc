#include <gtest/gtest.h>

#include "utils.hh"

#include <iterator>
#include <string>
#include <vector>

TEST(UtilsTest, FilteredRange)
{
    std::vector<int> v{0, 1, 5, -3, 3, 2, -5, 12};
    std::vector<int> expected{1, 5, 3, 2, 12};
    std::vector<int> to;
    auto fr =
        spyc::utils::make_filtered_range(v, [](const int& i) { return i > 0; });
    std::copy(std::begin(fr), std::end(fr), std::back_inserter(to));
    EXPECT_EQ(to.size(), 5);
    EXPECT_EQ(to, expected);
}
