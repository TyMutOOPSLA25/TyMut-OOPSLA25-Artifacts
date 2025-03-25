#include "Mutator_base.h"

/**
 * mut5
 * Change the initilization order of member variable
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
  class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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
 * mut8
 * Change access specifier of ctors/dtors
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
  class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
 * mut10
 * Declare ctors as explicit
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
  class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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

      std::set<string> ctornames;
      std::set<string> classnames;
  };
};

/**
 * mut11
 * Replace object initilization exp.
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
  class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
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
      std::vector<string> classnames;
  };
};

/**
 * mut16
 * Change the ‘=default/=delete’ specifier for ctors
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
  class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
 * mut20
 * "Insert into derived class ctor that uses base class For parameter"
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
  class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
 * mut24
 * Generate value initilization
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
  class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
 * mut25
 * Insert in_place initilization
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
  class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
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
 * mut44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
  class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
 * mut45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
  class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
 * mut46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
  class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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

/**
 * mut47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)

private:
  class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
 * mut51
 */ 
class MutatorFrontendAction_51 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(51)

private:
  class MutatorASTConsumer_51 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_51(Rewriter &R) : TheRewriter(R) {}
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
      map<string, vector<string>> recordCtorPramMap;
      unordered_set<string> inserted;
  };
};