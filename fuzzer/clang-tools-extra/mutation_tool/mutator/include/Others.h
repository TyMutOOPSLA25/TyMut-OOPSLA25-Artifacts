#include "Mutator_base.h"

/**
 * mut13
 * Insert constexpr static member value
 */ 
class MutatorFrontendAction_13 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(13)

private:
  class MutatorASTConsumer_13 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_13(Rewriter &R) : TheRewriter(R) {}
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
      std::vector<int> cur_classes_type; 
  };
};

/**
 * mut14
 * "Insert or remove ‘noexcept’ specifier for member Methods"
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
  class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
 * mut15
 * Generate overloaded template functions
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
  class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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

      std::set<string> template_methods;
  };
};

/**
 * mut19
 * Remove ‘=0’ for pure virtual function
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
  class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
 * mut26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
  class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
      bool recordsInstrumented = false;
  };
};

/**
 * mut27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
  class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
 * mut28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
  class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
 * mut48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
  class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
      map<string, string> recordInsMap;
      unordered_set<string> inserted;
  };
};

/**
 * mut49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
  class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
      map<string, pair<bool, pair<SourceLocation, string>>> recordInsMap;
      map<string, vector<string>> typeRecordsMap;
  };
};

/**
 * mut50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
  class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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