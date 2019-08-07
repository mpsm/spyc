CLANG_SYSROOT?= /usr/lib/llvm-8
CLANG_LIBDIR= $(CLANG_SYSROOT)/lib
CLANG_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/libclang*.a)))
LLVM_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/libLLVM*.a)))

# setup sources
SRCS= CodeModel.cc CodeVisitor.cc DotOutputter.cc Method.cc
TEST_SRCS:= test.cc $(SRCS)
SRCS+= spyc.cc

BUILDDIR= build

# g++ breaks build because of warnings caused by LLVM header files
CXX?= clang++
CFLAGS+= -Wall -Werror
ifeq ($(CXX),g++)
CFLAGS+= -Wno-comment -Wno-strict-aliasing
endif

OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.cc=.o))
OBJS+= $(OBJDIR)/gitversion.o
DEPS= $(OBJS:.o=.d)
TARGET= spyc

TEST_BUILDDIR= $(BUILDDIR)/test
TESTDIR= test

TEST_SRCS+= $(notdir $(wildcard $(TESTDIR)/*_test.cc))
TEST_OBJS= $(addprefix $(TEST_BUILDDIR)/, $(TEST_SRCS:.cc=.o))
TEST_DEPS= $(TEST_OBJS:.o=.d)
TEST_COVFILES= $(TEST_OBJS:.o=.gcda) $(TEST_OBJS:.o=.gcno)
TEST_TARGET= testall
TEST_CFLAGS= -g -O0 -coverage
TEST_CFLAGS+= -I $(SRCDIR)
TEST_LDFLAGS= -lgtest --coverage
TEST_COVINFO= $(TEST_BUILDDIR)/coverage.info
TEST_COVHTML= $(BUILDDIR)/coverage

CFLAGS+= -pthread
CFLAGS+= -I $(CLANG_SYSROOT)/include
CPPFLAGS+= -MMD -MP -MT $@ -MT $(@:.o=.d) -MF $(@:.o=.d)
CXXFLAGS+= -fno-rtti
LDFLAGS+= -L $(CLANG_LIBDIR)

# scan clang and LLVM libraries
define scan-libs
$(1)_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/lib$(1)*.a)))
LDFLAGS+= -Wl,--start-group $$($(1)_LIBS) -Wl,--end-group
endef
$(eval $(call scan-libs,clang))
$(eval $(call scan-libs,LLVM))

# add LLVM dependencies
LDFLAGS+= -pthread -lncurses -lz

# setup build type
BUILD_TYPE?= release
ifeq ($(BUILD_TYPE),release)
BT_CFLAGS+= -O3
else ifeq ($(BUILD_TYPE),debug)
BT_CFLAGS+= -O0 -g
TARGET:= $(TARGET)-debug
else
$(error "Invalid build type")
endif

OBJDIR= $(BUILDDIR)/$(BUILD_TYPE)
SRCDIR= src

all: coverage $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BUILDDIR)/$(TEST_TARGET): $(TEST_OBJS) | $(BUILDDIR)
	$(CXX) $^ $(LDFLAGS) $(TEST_LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(BT_CFLAGS) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(TEST_BUILDDIR)/%.o: $(SRCDIR)/%.cc | $(TEST_BUILDDIR)
	$(CXX) $(CPPFLAGS) $(TEST_CFLAGS) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(TEST_BUILDDIR)/%.o: $(TESTDIR)/%.cc | $(TEST_BUILDDIR)
	$(CXX) $(CPPFLAGS) $(TEST_CFLAGS) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR) $(OBJDIR) $(TEST_BUILDDIR):
	mkdir -p $@

ifneq ($(wildcard ./.git/index),)
$(SRCDIR)/gitversion.cc: .git/HEAD .git/index $(addprefix $(SRCDIR)/,$(SRCS)) Makefile
	echo "namespace spyc{const char *gitversion = \"$(shell git rev-parse --short HEAD)$(shell git diff --quiet || echo '-dirty')\";}" > $@
else
$(SRCDIR)/gitversion.cc:
	echo "namespace spyc{const char *gitversion = nullptr;}" > $@
endif

clean:
	rm -f $(OBJS)

distclean:
	rm -rf $(BUILDDIR)

coverage: $(TEST_COVHTML)

$(TEST_COVHTML): $(TEST_COVINFO)
	genhtml $< --output-directory $@

$(TEST_COVINFO): test
	lcov -q --capture --no-external --directory $(TEST_BUILDDIR) --base-directory . --output $@
	lcov -q --remove $@ -o $@ $(shell realpath $(TESTDIR))/*

test: $(BUILDDIR)/$(TEST_TARGET) | $(BUILDDIR)
	./$<

-include $(DEPS)
-include $(TEST_DEPS)

.PRECIOUS: $(DEPS)
.PHONY: test distclean coverage clean all
