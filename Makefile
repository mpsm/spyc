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

all: spyc

spyc: spyc.cc
	clang++ -I /usr/lib/llvm-8/include $< -O0 -g -o $@ -Wl,--start-group $(CLANG_LIBS) -Wl,--end-group -lLLVM

clean:
	rm -f spyc


