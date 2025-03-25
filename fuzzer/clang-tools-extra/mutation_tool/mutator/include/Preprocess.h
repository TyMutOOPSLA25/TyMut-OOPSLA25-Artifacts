#include "Mutator_base.h"

/**
 * mut1001
 * 
 */ 
class MutatorFrontendAction_1001 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1001)

private:
  class MutatorASTConsumer_1001 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_1001(Rewriter &R) : TheRewriter(R) {}
      void HandleTranslationUnit(ASTContext &Context) override;
    private:
      Rewriter &TheRewriter;
  };
  
  class Callback : public MatchFinder::MatchCallback {
    public:
      Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
      virtual void run(const MatchFinder::MatchResult &Result);
    private:
      Rewriter &Rewrite;

      set<string> visitiedRecord;
  };
};

/**
 * mut1002
 * 
 */ 
class MutatorFrontendAction_1002 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1002)

private:
  class MutatorASTConsumer_1002 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_1002(Rewriter &R) : TheRewriter(R) {}
      void HandleTranslationUnit(ASTContext &Context) override;
    private:
      Rewriter &TheRewriter;
  };
  
  class Callback : public MatchFinder::MatchCallback {
    public:
      Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
      virtual void run(const MatchFinder::MatchResult &Result);
    private:
      Rewriter &Rewrite;

      set<string> visitiedRecord;
  };
};

/**
 * mut1003
 * 
 */ 
class MutatorFrontendAction_1003 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1003)

private:
  class MutatorASTConsumer_1003 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_1003(Rewriter &R) : TheRewriter(R) {}
      void HandleTranslationUnit(ASTContext &Context) override;
    private:
      Rewriter &TheRewriter;
  };
  
  class Callback : public MatchFinder::MatchCallback {
    public:
      Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
      virtual void run(const MatchFinder::MatchResult &Result);
    private:
      Rewriter &Rewrite;

      set<string> visitiedRecord;
      string lastRecordName = "";
  };
};


/**
 * mut1005
 * 
 */ 
class MutatorFrontendAction_1005 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1005)

private:
  class MutatorASTConsumer_1005 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_1005(Rewriter &R) : TheRewriter(R) {}
      void HandleTranslationUnit(ASTContext &Context) override;
    private:
      Rewriter &TheRewriter;
  };
  
  class Callback : public MatchFinder::MatchCallback {
    public:
      Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
      virtual void run(const MatchFinder::MatchResult &Result);
    private:
      Rewriter &Rewrite;

      set<string> insertedRecord;
  };
};