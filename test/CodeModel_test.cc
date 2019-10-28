#include <gtest/gtest.h>

#include "Call.hh"
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

TEST(CodeModelTest, AddCall)
{
    spyc::CodeModel cm;

    ASSERT_EQ(cm.getCalls().size(), 0);

    auto foo = cm.getMethod({"foo", ""});
    auto bar = cm.getMethod({"bar", ""});

    cm.addCall(*foo, *bar);

    ASSERT_EQ(cm.getCalls().size(), 1U);

    auto rv = *cm.getCalls().begin();
    auto [caller, callee] = *cm.getCalls().begin();

    ASSERT_EQ(caller.get().getName(), "foo");
    ASSERT_EQ(callee.get().getName(), "bar");
}

TEST(CodeModelTest, FindMethods)
{
    spyc::CodeModel cm;

    ASSERT_EQ(cm.findMethodsByName("foo").size(), 0);

    auto foo = cm.getMethod({"foo", ""});
    auto bar = cm.getMethod({"bar", ""});

    auto found = cm.findMethodsByName("foo");

    ASSERT_EQ(found.size(), 1U);
    ASSERT_EQ(found[0], foo);

    auto foo2 = cm.getMethod({"foo", "baz.c"});

    ASSERT_NE(foo2, foo);

    auto found2 = cm.findMethodsByName("foo");

    ASSERT_EQ(found2.size(), 2U);
}