#include "../include/Preprocess.h"

// ========================================================================================================
#define MUT1001_OUTPUT 1

void MutatorFrontendAction_1001::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    
    if (recordDecl->isTemplated())
      return;

    auto recordName = recordDecl->getNameAsString();
    if (recordName == "")
      return;
    
    if (visitiedRecord.find(recordName) != visitiedRecord.end())
      return;
    else
      visitiedRecord.insert(recordName);

    #if MUT1001_OUTPUT
    llvm::errs() << "MUT1001: " << "Match record: " << recordName << "\n";
    #endif

    string insText = "";
    set<string> baseRecord = visitiedRecord;
    baseRecord.erase(recordName);
    bool hasBase = recordDecl->getNumBases() > 0;
    if (hasBase) { 
      for (const auto &base : recordDecl->bases()) {

        const CXXRecordDecl *baseDecl = base.getType()->getAsCXXRecordDecl();
        if (baseDecl) {
          string baseName = baseDecl->getNameAsString();
          if (baseRecord.find(baseName) != baseRecord.end())
            baseRecord.erase(baseName);

          #if MUT1001_OUTPUT
          llvm::errs() << "\t" << "Inherits from: " << baseName << "\n";
          #endif
        }
        
      }
    }

    for (auto rName : baseRecord) {
      if (insText != "")
        insText += ", ";
      insText += rName;
    }
    if (insText != "") {
      if (hasBase)
        insText = ", " + insText;
      else
        insText = ": " + insText;

      SourceRange range = recordDecl->getSourceRange();
      string oriText = stringutils::rangetoStr(SM, range);
      size_t braceOff = oriText.find('{');
      SourceLocation braceStartLoc = range.getBegin().getLocWithOffset(braceOff);
      SourceLocation insLoc = braceStartLoc;
      Rewrite.InsertText(insLoc, "/*pre1*/"+insText, false);

      #if MUT1001_OUTPUT
      llvm::errs() << "\t" << "Insert: " << insText << "\n";
      #endif
    }
    
  }
}

void MutatorFrontendAction_1001::MutatorASTConsumer_1001::HandleTranslationUnit(ASTContext &Context) {
    Callback callback(TheRewriter);

    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("recordDecl");
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT1002_OUTPUT 1

void MutatorFrontendAction_1002::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    
    if (recordDecl->isTemplated())
      return;

    auto recordName = recordDecl->getNameAsString();
    if (recordName == "")
      return;
    
    if (visitiedRecord.find(recordName) != visitiedRecord.end())
      return;
    else
      visitiedRecord.insert(recordName);

    #if MUT1002_OUTPUT
    llvm::errs() << "MUT1002: " << "Match record: " << recordName << "\n";
    #endif

    string insText = "\n";
    insText += recordName + "()=default;    ";
    insText += recordName + "(const " + recordName + "&)=default;    ";
    insText += recordName + "(" + recordName + " &&)=default;    ";
    insText += "constexpr " + recordName + "& operator=(const " + recordName + "&)=default;\n";

    SourceLocation insLoc = recordDecl->getEndLoc();
    Rewrite.InsertText(insLoc, "/*pre2*/"+insText, false, true);

    #if MUT1002_OUTPUT
    llvm::errs() << "\t" << "Insert: " << insText << "\n";
    #endif
  }
}

void MutatorFrontendAction_1002::MutatorASTConsumer_1002::HandleTranslationUnit(ASTContext &Context) {
    Callback callback(TheRewriter);

    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("recordDecl");
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT1003_OUTPUT 1

void MutatorFrontendAction_1003::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    
    if (recordDecl->isTemplated())
      return;

    auto recordName = recordDecl->getNameAsString();
    if (recordName == "")
      return;
    
    if (visitiedRecord.find(recordName) != visitiedRecord.end())
      return;
    else
      visitiedRecord.insert(recordName);

    #if MUT1003_OUTPUT
    llvm::errs() << "MUT1003: " << "Match record: " << recordName << "\n";
    #endif

    string insText = "\n";

    insText += "int pre3_a;  ";
    insText += "char* pre3_v1;  ";
    insText += "int* pre3_v2;  ";
    if (lastRecordName != "")
      insText += lastRecordName + " pre3_v3;  ";
    else
      lastRecordName = recordName;
    insText += "void pre3_func1(int para);  ";
    insText += "int pre3_func2() { return pre3_a; }";
    
    insText += "\n";

    SourceLocation insLoc = recordDecl->getEndLoc();
    Rewrite.InsertText(insLoc, "/*pre3*/"+insText, false, true);

    #if MUT1003_OUTPUT
    llvm::errs() << "\t" << "Insert: " << insText << "\n";
    #endif
  }
}

void MutatorFrontendAction_1003::MutatorASTConsumer_1003::HandleTranslationUnit(ASTContext &Context) {
    Callback callback(TheRewriter);

    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("recordDecl");
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT1005_OUTPUT 1

void MutatorFrontendAction_1005::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    
    if (recordDecl->isTemplated())
      return;

    auto recordName = recordDecl->getNameAsString();
    if (recordName == "")
      return;
    
    #if MUT1005_OUTPUT
    llvm::errs() << "MUT1005: " << "Match record: " << recordName << "\n";
    #endif

    string insText4Derived = "";

    bool hasBase = recordDecl->getNumBases() > 0;
    if (hasBase) { 
      for (const auto &base : recordDecl->bases()) {

        const CXXRecordDecl *baseDecl = base.getType()->getAsCXXRecordDecl();
        if (baseDecl) {
          string baseName = baseDecl->getNameAsString();
          if (insertedRecord.find(baseName) != insertedRecord.end())
            continue;
          else
            insertedRecord.insert(baseName);

          if (! baseDecl->hasDefinition())
            continue;
          
          string insText = "virtual void pre5_func_" + baseName + "();\n";
          insText4Derived += "virtual void pre5_func_" + baseName + "()override;\n";

          SourceLocation insLoc = baseDecl->getEndLoc();
          Rewrite.InsertText(insLoc, "/*pre4*/"+insText, false, true);
          #if MUT1005_OUTPUT
          llvm::errs() << "\t" << "Insert base: " << insText << "\n";
          #endif
        }
      }
    }
    if (insText4Derived != "") {
      if (insertedRecord.find(recordName) != insertedRecord.end())
        return;
      else
        insertedRecord.insert(recordName);
      SourceLocation insLoc = recordDecl->getEndLoc();
      Rewrite.InsertText(insLoc, "/*pre4*/\n"+insText4Derived, false, true);

      #if MUT1005_OUTPUT
      llvm::errs() << "\t" << "Insert: " << insText4Derived << "\n";
      #endif
    }
    
  }
}

void MutatorFrontendAction_1005::MutatorASTConsumer_1005::HandleTranslationUnit(ASTContext &Context) {
    Callback callback(TheRewriter);

    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("recordDecl");
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}