TARGET= spyc

CLANG_SYSROOT?= /usr/lib/llvm-8
CLANG_LIBDIR= $(CLANG_SYSROOT)/lib
CLANG_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/libclang*.a)))
LLVM_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/libLLVM*.a)))

SRCS= CodeModel.cc CodeVisitor.cc DotOutputter.cc Method.cc spyc.cc
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.cc=.o))
OBJS+= $(OBJDIR)/gitversion.o
DEPS= $(OBJS:.o=.d)

CXX?= clang++
CFLAGS+= -Wall -Werror
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

# g++ breaks build because of warnings caused by LLVM header files
ifeq ($(CXX),g++)
CFLAGS+= -Wno-comment -Wno-strict-aliasing
endif

# setup build type
BUILD_TYPE?= release
ifeq ($(BUILD_TYPE),release)
CFLAGS+= -O3
else ifeq ($(BUILD_TYPE),debug)
CFLAGS+= -O0 -g
TARGET:= $(TARGET)-debug
else
$(error "Invalid build type")
endif

BUILDDIR= build
OBJDIR= $(BUILDDIR)/$(BUILD_TYPE)
SRCDIR= src

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
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

-include $(DEPS)

.PRECIOUS: $(DEPS)
.PHONY: distclean clean all
