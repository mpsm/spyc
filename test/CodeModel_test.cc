#include <gtest/gtest.h>

#include "CodeModel.hh"
#include "Method.hh"

TEST(CodeModelTest, CreateUnique)
{
    spyc::CodeModel cm;

    ASSERT_EQ(cm.getFunctions().size(), 0U);

    auto m = cm.getMethod(spyc::Method::ID{"", "aaa"});
    auto n = cm.getMethod(spyc::Method::ID{"", "aaa"});

    ASSERT_NE(m, nullptr);
    ASSERT_EQ(cm.getFunctions().size(), 1U);
    ASSERT_EQ(m, n);
}