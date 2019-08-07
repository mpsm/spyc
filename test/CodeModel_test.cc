#include <gtest/gtest.h>

#include "CodeModel.hh"

TEST (CodeModelTest, CreateUnique)
{
    spyc::CodeModel cm;

    ASSERT_EQ(cm.getFunctions().size(), 0);

    auto m = cm.createFunction("aaa");
    auto n = cm.createFunction("aaa");

    ASSERT_NE(m, nullptr);
    ASSERT_EQ(cm.getFunctions().size(), 1);
    ASSERT_EQ(m, n);
}