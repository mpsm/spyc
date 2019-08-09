#include <gtest/gtest.h>

#include "Method.hh"

#include <memory>
#include <string>

TEST (MethodTest, Init)
{
    spyc::Method m("testMethod");
    EXPECT_EQ(m.getName(), std::string("testMethod"));
}

TEST (MethodTest, Equality)
{
    spyc::Method m1("aaa"), m2("bbb"), m3("aaa");
    EXPECT_EQ(m1, m3);
    EXPECT_NE(m1, m2);
    EXPECT_NE(m2, m3);
}

TEST (MethodTest, Link)
{
    auto sm1 = std::make_shared<spyc::Method>("aaa");
    auto sm2 = std::make_shared<spyc::Method>("bbb");

    spyc::linkMethods(sm1, sm2);

    EXPECT_EQ(sm1->getCallees().size(), 1);
    EXPECT_EQ(sm1->getCallers().size(), 0);
    EXPECT_EQ(sm2->getCallees().size(), 0);
    EXPECT_EQ(sm2->getCallers().size(), 1);

    EXPECT_EQ((*sm1->getCallees().begin()).lock(), sm2);
    EXPECT_EQ((*sm2->getCallers().begin()).lock(), sm1);
}

TEST (MethodTest, DoubleLink)
{
    auto sm1 = std::make_shared<spyc::Method>("aaa");
    auto sm2 = std::make_shared<spyc::Method>("bbb");

    spyc::linkMethods(sm1, sm2);
    spyc::linkMethods(sm1, sm2);
    
    EXPECT_EQ(sm1->getCallees().size(), 1);
    EXPECT_EQ(sm1->getCallers().size(), 0);
}

TEST (MethodTest, SimpleConstruct)
{
    auto m = spyc::Method("name");

    EXPECT_EQ(m.getName(), "name");
    EXPECT_EQ(m.getFileName(), "");
}