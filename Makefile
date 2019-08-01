CLANG_LIBS= -lclangAnalysis -lclangApplyReplacements -lclangARCMigrate -lclangAST \
	    -lclangASTMatchers -lclangBasic -lclangChangeNamespace -lclangCodeGen \
	    -lclangCrossTU -lclangDaemon -lclangDoc -lclangDriver -lclangDynamicASTMatchers \
	    -lclangEdit -lclangFormat -lclangFrontend -lclangFrontendTool -lclangHandleCXX \
	    -lclangHandleLLVM -lclangIncludeFixer -lclangIncludeFixerPlugin -lclangIndex \
	    -lclangLex -lclangMove -lclangParse -lclangQuery -lclangReorderFields \
	    -lclangRewrite -lclangRewriteFrontend -lclangSema -lclangSerialization \
	    -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangStaticAnalyzerFrontend \
	    -lclangTidy -lclangTidyAbseilModule -lclangTidyAndroidModule -lclangTidyBoostModule \
	    -lclangTidyBugproneModule -lclangTidyCERTModule -lclangTidyCppCoreGuidelinesModule \
	    -lclangTidyFuchsiaModule -lclangTidyGoogleModule -lclangTidyHICPPModule \
	    -lclangTidyLLVMModule -lclangTidyMiscModule -lclangTidyModernizeModule \
	    -lclangTidyMPIModule -lclangTidyObjCModule -lclangTidyPerformanceModule \
	    -lclangTidyPlugin -lclangTidyPortabilityModule -lclangTidyReadabilityModule \
	    -lclangTidyUtils -lclangTidyZirconModule -lclangTooling -lclangToolingASTDiff \
	    -lclangToolingCore -lclangToolingInclusions -lclangToolingRefactor

TARGET= spyc

OBJDIR= build
SRCDIR= src

SRCS= CodeModel.cc CodeVisitor.cc DotOutputter.cc Method.cc spyc.cc
OBJS= $(addprefix $(OBJDIR)/, $(SRCS:.cc=.o))

CXX= clang++

CFLAGS+= -O0 -g
CXXFLAGS+= -I /usr/lib/llvm-8/include
LDFLAGS+= -Wl,--start-group $(CLANG_LIBS) -Wl,--end-group -lLLVM

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJS)

distclean:
	rm -rf $(OBJDIR)

