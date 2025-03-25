#include "../include/well_formed.h"

// ==========================================================================================
std::vector<const clang::CXXConstructExpr *> AllCXXConstructExpr;
std::vector<const clang::FieldDecl *> AllRecordField;

void Callback_GetAllClassInstance::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("constructExpr")) {
    if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CE->getLocation()))
      return;
    if (const CXXConstructorDecl *CtorDecl = CE->getConstructor()) {
      if (const CXXRecordDecl *ClassDecl = CtorDecl->getParent()) {
        llvm::errs() << "CXXConstructExpr found: " << stringutils::rangetoStr(SM, CE->getSourceRange()) << "[" << ClassDecl->getNameAsString() << "]" << "\n";
        AllCXXConstructExpr.push_back(CE);
      }
    }
  }
  if (const FieldDecl *FD = Result.Nodes.getNodeAs<FieldDecl>("recordField")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    auto fieldTypeName = FD->getType()->getAs<RecordType>()->getDecl()->getQualifiedNameAsString();
    llvm::errs() << "FieldDecl found: " << FD->getNameAsString() << " [" << fieldTypeName << "]\n";
    AllRecordField.push_back(FD);
  }
}

void getAllClassInstance(ASTContext &Context, Rewriter &Rewriter) {
  AllCXXConstructExpr.clear();
  AllRecordField.clear();

  MatchFinder matchFinder1;
  StatementMatcher matcher1 = cxxConstructExpr(unless(hasAncestor(castExpr()))).bind("constructExpr");
  auto matcher2 = fieldDecl(hasType(recordType())).bind("recordField");
  Callback_GetAllClassInstance callback1(Rewriter);
  matchFinder1.addMatcher(matcher1, &callback1);
  matchFinder1.addMatcher(matcher2, &callback1);
  matchFinder1.matchAST(Context);
}

bool hasInstanceOfClass(const clang::CXXRecordDecl *ClassDecl) {
  for (auto construExpr : AllCXXConstructExpr) {
    string construExpr_str = construExpr->getConstructor()->getParent()->getNameAsString();
    if (construExpr_str == ClassDecl->getNameAsString()) {
      return true;
    }
  }
  for (auto field : AllRecordField) {
    auto fieldTypeName = field->getType()->getAs<RecordType>()->getDecl()->getQualifiedNameAsString();
    if (fieldTypeName == ClassDecl->getNameAsString()) {
      return true;
    }
  }

  return false;
}

// ==========================================================================================
std::unordered_map<const clang::CXXRecordDecl *, std::vector<const clang::CXXRecordDecl *>> InheritanceMap;

void Callback_GetInheritanceMap::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->hasDefinition() == false)
      return;
    if (DL->isLambda())
      return;

    auto bases = DL->bases();
    for (auto base : bases) {
      auto baseClass = base.getType()->getAsCXXRecordDecl();
      if (baseClass) {
        InheritanceMap[baseClass].push_back(DL);
      }
    }
  }
}

void getInheritanceMap(ASTContext &Context, Rewriter &Rewriter) {
  InheritanceMap.clear();

  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
  Callback_GetInheritanceMap callback(Rewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

void dumpInheritanceMap() {
  for (auto &entry : InheritanceMap) {
    llvm::errs() << "Base class: " << entry.first->getNameAsString() << "\n";
    for (auto &derived : entry.second) {
      llvm::errs() << "\tDerived class: " << derived->getNameAsString() << "\n";
    }
  }
}

std::vector<const clang::CXXRecordDecl *> getDerivedClasses(const clang::CXXRecordDecl *BaseClass) {
  std::vector<const clang::CXXRecordDecl *> DerivedClasses;
  if (InheritanceMap.find(BaseClass) != InheritanceMap.end()) {
    DerivedClasses = InheritanceMap[BaseClass];
  }
  return DerivedClasses;
}