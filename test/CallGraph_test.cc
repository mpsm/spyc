#include <gtest/gtest.h>

#include "CallGraph.hh"
#include "Method.hh"

#include <string>

TEST(CallGraph, root)
{
    spyc::Method m("foo");
    spyc::CallGraph cg(m);

    ASSERT_EQ(cg.getRoot(), m);
}

TEST(CallGraph, getNodeCount)
{
    spyc::Method m("foo"), m1("bar");
    spyc::linkMethods(m, m1);

    spyc::CallGraph cg(m1), cg1(m);

    ASSERT_EQ(cg.getMethodCount(), 1U);
    ASSERT_EQ(cg1.getMethodCount(), 2U);
}

TEST(CallGraph, cycle)
{
    spyc::Method m("foo");
    spyc::linkMethods(m, m);

    spyc::CallGraph cg(m);

    ASSERT_EQ(cg.getMethodCount(), 1U);
    ASSERT_EQ(cg.getCallCount(), 1U);
    ASSERT_TRUE(cg.hasCall(m, m));
}

TEST(CallGraph, bigCycle)
{
    spyc::Method m1("foo"), m2("bar"), m3("baz");

    spyc::linkMethods(m1, m2);
    spyc::linkMethods(m2, m3);
    spyc::linkMethods(m3, m1);

    spyc::CallGraph cg(m2);

    ASSERT_EQ(cg.getMethodCount(), 3U);
}

TEST(CallGraph, multiCall)
{
    spyc::Method m1("foo"), m2("bar"), m3("baz");

    spyc::linkMethods(m1, m2);
    spyc::linkMethods(m2, m3);
    spyc::linkMethods(m1, m3);

    spyc::CallGraph cg(m1);

    ASSERT_EQ(cg.getMethodCount(), 3U);
    ASSERT_EQ(cg.getCallCount(), 3U);
}

TEST(CallGraph, hasNode)
{
    spyc::Method m("foo");
    spyc::CallGraph cg(m);

    ASSERT_TRUE(cg.hasMethod(m));
}

TEST(CallGraph, hasVertex)
{
    spyc::Method foo("foo");
    spyc::Method bar("bar");

    spyc::linkMethods(foo, bar);
    spyc::CallGraph cg(foo);

    ASSERT_EQ(cg.getCallCount(), 1U);
    ASSERT_TRUE(cg.hasCall(foo, bar));
    ASSERT_FALSE(cg.hasCall(bar, foo));
}