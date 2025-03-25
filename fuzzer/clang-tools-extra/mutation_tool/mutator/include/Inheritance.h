#include "Mutator_base.h"

/**
 * mut1
 * Change the inherit mode of a certain base class
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
  class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
  };
};

/**
 * mut2
 * Insert user-defined dtor for a base class
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)

private:
  class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
  };
};

/**
 * mut3
 * Insert ‘using Base::var’ for a derived class
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
  class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
      std::vector<const clang::CXXRecordDecl *> cur_classes;
  };
};

/**
 * mut4
 * Insert resue declaration of base class’s constructor
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
  class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
      std::vector<const clang::CXXRecordDecl *> cur_classes;
  };
};

/**
 * mut7
 * Create empty base classes
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
  class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
      int targets = 0;
  };
};

/**
 * mut18
 * Insert ‘virtual’ for inheritance
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
  class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
  };
};

/**
 * mut38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
  class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
  };
};

/**
 * mut39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
  class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
  };
};

/**
 * mut54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)

private:
  class MutatorASTConsumer_54 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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

      set<string> visitedRecord;
      string lastRecordName = "";
  };
};