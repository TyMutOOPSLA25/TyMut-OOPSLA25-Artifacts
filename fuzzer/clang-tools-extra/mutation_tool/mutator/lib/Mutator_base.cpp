#include "../include/Mutator_base.h"

void MutatorFrontendAction::EndSourceFileAction() {
  SourceManager &SM = TheRewriter.getSourceMgr();
  llvm::errs() << "** EndSourceFileAction for: "
                << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

  std::string filePath = SM.getFileEntryForID(SM.getMainFileID())->getName().str();
  std::error_code EC;
  llvm::raw_fd_ostream outFile(filePath, EC, llvm::sys::fs::OF_None);
  if (EC) {
      llvm::errs() << "Error opening file: " << EC.message() << "\n";
      return;
  }
  
  TheRewriter.getEditBuffer(SM.getMainFileID()).write(outFile);
}