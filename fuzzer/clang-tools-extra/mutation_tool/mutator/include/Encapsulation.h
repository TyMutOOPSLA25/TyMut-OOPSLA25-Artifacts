#include "Mutator_base.h"

/**
 * mut40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
  class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
 * mut6
 * Declare friend classes
 */ 
class MutatorFrontendAction_6 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)

private:
  class MutatorASTConsumer_6 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
      std::set<string> cur_classnames;
  };
};

/**
 * mut9
 * Insert a lambda expression for member methods
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
  class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
 * mut17
 * Generate ‘using enum’s
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
  class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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

      std::vector<std::string> enumerate_set;
      std::set <std::string> classnames;
      int first=0;
  };
};

/**
 * mut21
 * Generate friend functions
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
  class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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

      std::set<string> cur_classnames;
      std::vector<string> candidate_methods;
      std::vector<int> has_extra_spec;
  };
};

/**
 * mut29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)

private:
  class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
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

      std::map<std::string, std::string> instrumentedDeclMap;
  };
};

/**
 * mut30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
  class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
 * mut41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
  class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
 * mut42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
  class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
      unordered_set<string> viewed; 
  };
};