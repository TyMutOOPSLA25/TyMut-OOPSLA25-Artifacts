#include "mutator/Mutator.h"
// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

static cl::opt<int> Mutator_selector("mutator",cl::desc("Selected mutator"),cl::cat(MyToolCategory));
static cl::opt<string> Sourcedir("sourcedir",cl::desc("Directory path of the test source files"),cl::value_desc("directory_path"),cl::cat(MyToolCategory)) ;

string SRCDIR="/home/rush/code/simple_tests/";


// ID 1001 and later are preprocessing

/*
* well-formed info
* 0 : well-formed without validate
* 1 : well-formed after validate
* -1: not-well-formed after validate
*/
int WELL_FORMED_INFO = 0;

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }

  auto mutator=int(Mutator_selector);
  SRCDIR=Sourcedir.c_str();


  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  RefactoringTool Rtool(OptionsParser.getCompilations(),OptionsParser.getSourcePathList());

  llvm::errs() << "Mutator: " << mutator << "\n";

  std::set<int> mutator_set = {18, 21, 6, 1, 8, 12};
  if (mutator_set.find(mutator) != mutator_set.end()) {
    WELL_FORMED_INFO = 1;
  }
  clang::tooling::FrontendActionFactory * FrontendFactory;
  FrontendFactory = getMutatorNewFrontendActionFactory(mutator);
  Rtool.run(FrontendFactory);
  return WELL_FORMED_INFO;
}
