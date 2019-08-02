TARGET= spyc

CLANG_SYSROOT?= /usr/lib/llvm-8
CLANG_LIBDIR= $(CLANG_SYSROOT)/lib
CLANG_LIBS= $(patsubst lib%.a,-l%,$(notdir $(wildcard $(CLANG_LIBDIR)/libclang*.a)))

OBJDIR= build
SRCDIR= src

SRCS= CodeModel.cc CodeVisitor.cc DotOutputter.cc Method.cc spyc.cc
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.cc=.o))
DEPS= $(OBJS:.o=.d)

CXX?= clang++
CFLAGS+= -Wall -Werror
CPPFLAGS+= -MMD -MP -MT $@ -MT $(@:.o=.d) -MF $(@:.o=.d)
CXXFLAGS+= -I $(CLANG_SYSROOT)/include
LDFLAGS+= -L $(CLANG_LIBDIR)
LDFLAGS+= -Wl,--start-group $(CLANG_LIBS) -Wl,--end-group -lLLVM

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
else
$(error "Invalid build type")
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJS)

distclean:
	rm -rf $(OBJDIR)

-include $(DEPS)

.PRECIOUS: $(DEPS)
.PHONY: distclean clean all
