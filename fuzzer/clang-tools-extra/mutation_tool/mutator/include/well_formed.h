#include "../utils.h"

// =========================================================================
class Callback_GetAllClassInstance : public MatchFinder::MatchCallback {
  public:
    Callback_GetAllClassInstance(Rewriter &Rewrite) : Rewrite(Rewrite) {}
    virtual void run(const MatchFinder::MatchResult &Result);
  private:
    Rewriter &Rewrite;
};

void getAllClassInstance(ASTContext &Context, Rewriter &Rewrite);

bool hasInstanceOfClass(const clang::CXXRecordDecl *ClassDecl);

// =========================================================================
extern std::unordered_map<const clang::CXXRecordDecl *, std::vector<const clang::CXXRecordDecl *>> InheritanceMap;

class Callback_GetInheritanceMap : public MatchFinder::MatchCallback {
  public:
    Callback_GetInheritanceMap(Rewriter &Rewrite) : Rewrite(Rewrite) {}
    virtual void run(const MatchFinder::MatchResult &Result);
  private:
    Rewriter &Rewrite;
};

void getInheritanceMap(ASTContext &Context, Rewriter &Rewrite);
void dumpInheritanceMap();

std::vector<const clang::CXXRecordDecl *> getDerivedClasses(const clang::CXXRecordDecl *BaseClass);