#include "Mutator_base.h"

/**
 * mut12
 * Expand useless template type parameter
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
  class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
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
 * mut22
 * Generate templated derived classes
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
  class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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

      std::set<string> classnames;
  };
};

/**
 * mut23
 * Generate alias for template class/function usage
 */ 
class MutatorFrontendAction_23 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(23)

private:
  class MutatorASTConsumer_23 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_23(Rewriter &R) : TheRewriter(R) {}
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

      int cnt=1;
      std::vector<string> classnames;
  };
};

/**
 * mut31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
  class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
 * mut43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
  class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
 * mut52
 * 
 */ 
class MutatorFrontendAction_52 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(52)

private:
  class MutatorASTConsumer_52 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_52(Rewriter &R) : TheRewriter(R) {}
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
      unordered_set<string> inserted;
  };
};

/**
 * mut53
 * 
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
  MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
  class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
      MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
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

      inline const ClassTemplateSpecializationDecl *getCTSD_implicit(const Type *ty) {
        if (const auto *recordTy = ty->getAs<RecordType>())
          if (const auto *ctsd = dyn_cast<ClassTemplateSpecializationDecl>(recordTy->getDecl()))
            if (ctsd->getSpecializationKind() == TemplateSpecializationKind::TSK_ImplicitInstantiation)
              return ctsd;
        return nullptr;
      }

      pair<bool, string> isAllBasic(const TemplateArgumentList &args);

      unordered_set<string> inserted;
      void insertCtor(const ClassTemplateSpecializationDecl *ctsd);
  };
};