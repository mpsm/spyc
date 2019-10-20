#include <gtest/gtest.h>

#include "Method.hh"

#include <string>

TEST(MethodTest, Init)
{
    spyc::Method m("testMethod");
    EXPECT_EQ(m.getName(), std::string("testMethod"));
}

TEST(MethodTest, Equality)
{
    spyc::Method m1("aaa"), m2("bbb"), m3("aaa");
    EXPECT_EQ(m1, m3);
    EXPECT_NE(m1, m2);
    EXPECT_NE(m2, m3);
}

TEST(MethodTest, Link)
{
    spyc::Method m1("aaa"), m2("bbb");

    spyc::linkMethods(m1, m2);

    EXPECT_EQ(m1.getCallees().size(), 1U);
    EXPECT_EQ(m1.getCallers().size(), 0U);
    EXPECT_EQ(m2.getCallees().size(), 0U);
    EXPECT_EQ(m2.getCallers().size(), 1U);

    EXPECT_EQ((*m1.getCallees().begin()).get(), m2);
    EXPECT_EQ((*m2.getCallers().begin()).get(), m1);
}

TEST(MethodTest, DoubleLink)
{
    spyc::Method m1("aaa"), m2("bbb");

    spyc::linkMethods(m1, m2);
    spyc::linkMethods(m1, m2);

    EXPECT_EQ(m1.getCallees().size(), 1U);
    EXPECT_EQ(m1.getCallers().size(), 0U);
}

TEST(MethodTest, SimpleConstruct)
{
    auto m = spyc::Method("name");

    EXPECT_EQ(m.getName(), "name");
    EXPECT_EQ(m.getFileName(), "");
}