#pragma once
#include "../utils.h"
#include "well_formed.h"


class MutatorFrontendAction : public ASTFrontendAction {
public:
  void EndSourceFileAction() override;
private:
protected:
  Rewriter TheRewriter;
  class MutatorASTConsumer : public ASTConsumer {
  };
};

#define MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(m)\
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,\
                                                 StringRef file) override {\
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());\
    return std::make_unique<MutatorASTConsumer_##m>(TheRewriter);\
  }


const map<string, string> basicTypeValMap = {
  {"int", "42"},
  {"bool", "false"},
  {"char", "\'c\'"},
};

// well-formed info
extern int WELL_FORMED_INFO;
