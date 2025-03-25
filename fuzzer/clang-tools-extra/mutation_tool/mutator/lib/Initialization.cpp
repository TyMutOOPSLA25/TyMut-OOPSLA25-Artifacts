#include "../include/Initialization.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Notrivialctor")) {
    if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
      return;
    // if(CD-)
    auto inits = CD->inits();
    std::vector<string> init_codes;
    for (auto init : inits) {
      init_codes.push_back(stringutils::rangetoStr(*(Result.SourceManager),
                                                   init->getSourceRange()));
    }
    if (init_codes.size() > 1) {
      std::rotate(init_codes.begin(), init_codes.begin() + 1, init_codes.end());
    }
    int index = 0;
    for (auto init : inits) {
      init_codes[index] = "/*mut5*/" + init_codes[index];
      llvm::outs() << init_codes[index] << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()),
                          init_codes[index]);
      index++;
    }
  }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructorDecl(has(cxxCtorInitializer())).bind("Notrivialctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto ctors = DL->ctors();
    for (auto ctor : ctors) {
      if (ctor->isImplicit())
        continue;
      auto acc = ctor->getAccess();
      if (acc != AccessSpecifier::AS_private &&
          acc != AccessSpecifier::AS_protected &&
          acc != AccessSpecifier::AS_public)
        continue;
      int judge = getrandom::getRandomIndex(1);
      string newAcc = getrandom::getRandomAccessSepcifier();
      string replaceText = "\n/*mut8*/" + newAcc + ":\n";
      Rewrite.ReplaceText(ctor->getBeginLoc(), 0, replaceText);
      llvm::errs() << "\t" << "change ctor access to " << newAcc << "\n";
      
      // well-formed validate
      if (newAcc == "private") {
        if (hasInstanceOfClass(DL)) {
          llvm::errs() << "MUT8: " << "hasInstanceOfClass" << "\n";
          WELL_FORMED_INFO = -1;
        }
      }
    }
    auto dtor = DL->getDestructor();
    int judge_dtor = getrandom::getRandomIndex(1);
    llvm::outs() << judge_dtor << '\n';
    if (judge_dtor && dtor && !dtor->isImplicit()) {
      auto acc = dtor->getAccess();
      if (acc != AccessSpecifier::AS_private &&
        acc != AccessSpecifier::AS_protected &&
        acc != AccessSpecifier::AS_public)
        return;
      string newAcc = getrandom::getRandomAccessSepcifier();
      string replaceText = "\n/*mut8*/" + newAcc + ":\n";
      Rewrite.ReplaceText(dtor->getBeginLoc(), 0, replaceText);
      llvm::errs() << "\t" << "change dtor access to " << newAcc << "\n";
    }
  }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    Context.getTranslationUnitDecl()->dump();
    // well-formed validate
    getAllClassInstance(Context, TheRewriter);

    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    // DL->dump();
    if (DL->isCompleteDefinition() == false)
      return;
    if (classnames.count(DL->getNameAsString()))
      return;
    classnames.insert(DL->getNameAsString());
    auto ctors = DL->ctors();
    for (auto ctor : ctors) {
      auto ctorname = ctor->getNameAsString();
      if (ctornames.count(ctorname))
        continue;
      ctornames.insert(ctorname);
      llvm::outs() << ctorname << '\n';
      if (ctor->isImplicit() || ctor->isExplicit() || ctor->isTemplateDecl())
        continue;
      int judge = getrandom::getRandomIndex(1);
      // llvm::outs()<<judge<<'\n';
      if (judge) {
        Rewrite.ReplaceText(ctor->getBeginLoc(), 0, "/*mut10*/explicit ");
      }
    }
  }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *OD = Result.Nodes.getNodeAs<clang::DeclStmt>("Objectdecl")) {
    if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   OD->getBeginLoc()))
      return;
    auto decl = stringutils::rangetoStr(*(Result.SourceManager), OD->getSourceRange());
    bool judgeclass = false;
    for (auto classname : classnames) {
      if (decl.find(classname) != string::npos) {
        judgeclass = true;
        break;
      }
    }
    if (judgeclass) {
      llvm::outs() << decl << '\n';
      if (decl.find('(') != string::npos) {
        std::replace(decl.begin(), decl.end(), '(', '{');
        std::replace(decl.begin(), decl.end(), ')', '}');
        decl = "/*mut11*/" + decl;
      } else if (decl.find('{') == string::npos) {
        decl.insert(decl.rfind(';'), "{}");
        decl = "/*mut11*/" + decl;
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(OD->getSourceRange()), decl);
  } else if (auto CL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    // llvm::outs()<<CL->getNameAsString()<<'\n';
    classnames.push_back(CL->getNameAsString());
  }
}

void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto decl_matcher = declStmt().bind("Objectdecl");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    // DL->dump();
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto ctors = DL->ctors();
    for (auto ctor : ctors) {
      if (ctor->isImplicit())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ctor->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.rfind("default") != string::npos) {
        content.replace(content.rfind("default"), 7, "/*mut16*/delete");
      } else if (content.rfind("delete") != string::npos) {
        content.replace(content.rfind("delete"), 6, "/*mut16*/default");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ctor->getSourceRange()), content);
    }
  }
}

void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
  string acc_sep;
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    cur_classes.push_back(CL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false || DL->getNumBases() != 1)
      return;
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      if (DL->isDerivedFrom(cur_classes[j])) {
        auto baseclass = cur_classes[j];
        if (baseclass->isTrivial()) {
          std::string function_name = DL->getNameAsString();
          std::string param_name = baseclass->getNameAsString();
          acc_sep = "\n/*mut20*/" + function_name + "(" + param_name + "){}\n";
          llvm::outs() << acc_sep;
          
          Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
        }
      }
    }
  }
}

void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Inderived")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    llvm::outs() << CL->getNameAsString() << '\n';
    if (CL->isCompleteDefinition() && CL->isAggregate()) {
      cur_classes.push_back(CL);
    }
  } else if (auto *FC =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
    if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FC->getLocation()))
      return;
    llvm::outs() << FC->getNameAsString() << '\n';
    if (FC->isImplicit())
      return;
    if (cur_classes.empty())
      return;
    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto target = cur_classes[index];
    auto start = "/*mut24*/" + target->getNameAsString() + '{';
    std::vector<string> init_statments;
    for (auto field : target->fields()) {
      auto type = field->getType();
      if (type.getTypePtr()->isIntegerType())
        init_statments.push_back("." + field->getNameAsString() + "=0");
      else if (type.getTypePtr()->isAnyCharacterType())
        init_statments.push_back("." + field->getNameAsString() + "=\'c\'");
      else if (type.getTypePtr()->isArrayType() ||
               type.getTypePtr()->isStructuralType())
        init_statments.push_back("." + field->getNameAsString() + "={}");
    }
    if (init_statments.empty())
      return;
    for (std::size_t j = 0; j < init_statments.size(); ++j) {
      if (j == 0)
        start += init_statments[j];
      else {
        size_t judge = getrandom::getRandomIndex(1);
        if (judge) {
          start += ',' + init_statments[j];
        }
      }
    }
    start += "};\n";

    Rewrite.ReplaceText(FC->getBodyRBrace(), 0, start);
  }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto inderived_matcher = cxxRecordDecl(unless(hasAnyBase(hasType(cxxRecordDecl())))).bind("Inderived");
    auto func_matcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inderived_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    for (auto field : DL->fields()) {
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             field->getSourceRange());
      auto judge = getrandom::getRandomIndex(1);
      llvm::outs() << judge << '\n';
      if (judge)
        continue;
      if (content.find("=") == string::npos) {
        llvm::outs() << content << '\n';
        if (field->getType()->isIntegerType())
          content += "/*mut25*/=0";
        else if (field->getType()->isCharType())
          content += "/*mut25*/=\' \'";
        else if (field->getType()->isArrayType())
          content += "/*mut25*/={}";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(field->getSourceRange()), content);
      }
    }
  }
}

void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CallExpr *call = Result.Nodes.getNodeAs<CallExpr>("functionCall")) {
    if (!call)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(call->getBeginLoc())) 
      return;

    #if MUT44_OUTPUT
    llvm::errs() << "MUT44: " << "Match function call: " << stringutils::rangetoStr(SM, call->getSourceRange()) << "\n";
    #endif

    const FunctionDecl *funcDecl = call->getDirectCallee();

    if (funcDecl) {
      bool hasClassTypeArgument = false;

      for (unsigned i = 1; i < call->getNumArgs(); ++i) {
        const Expr *arg = call->getArg(i);
        if (const auto *argType = arg->getType().getTypePtrOrNull()) {
          if (argType->isRecordType()) {
            if (const CXXRecordDecl *record = argType->getAsCXXRecordDecl()) {
              if (record->isTemplated() || record->isLambda()) {
                continue;
              }
              SourceRange argRange = arg->getSourceRange();
              string argText = stringutils::rangetoStr(SM, argRange);
              string recordName = record->getNameAsString();

              string repText = recordName + "{}";

              if (argText != repText) {
                repText = "/*mut44*/" + repText;

                Rewrite.ReplaceText(argRange, repText);

                #if MUT44_OUTPUT
                llvm::errs() << "\t" << "replace [" << stringutils::rangetoStr(SM, argRange) 
                              << "] " << "to [" << repText << "]\n";
                #endif
              }
            }
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(unless(lambdaExpr())).bind("functionCall");;
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const FunctionDecl *funcDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl")) {
    if (!funcDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getBeginLoc())) 
      return;

    if (funcDecl->isExplicitlyDefaulted()) {
      #if MUT45_OUTPUT
      llvm::errs() << "MUT45: " << "Match function declaration: " << stringutils::rangetoStr(SM, funcDecl->getSourceRange()) << "\n";
      #endif
      SourceRange oriRange = funcDecl->getSourceRange();
      string oriText = stringutils::rangetoStr(SM, oriRange);
      assert(oriText.find("default") != string::npos && "[MUTATION ASSERT] Mut45: should have default keyword.\n");
      stringutils::strReplace(oriText, "default", "/*mut45*/delete");
      string repText = oriText;
      Rewrite.ReplaceText(oriRange, repText);
    }
  }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const MemberExpr *memberExpr = Result.Nodes.getNodeAs<MemberExpr>("intMemberUsage")) {
    if (!memberExpr)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(memberExpr->getBeginLoc())) 
      return;

    SourceRange exprRange = memberExpr->getSourceRange();
    string oriText = stringutils::rangetoStr(SM, exprRange);
    
    QualType exprType = memberExpr->getType();
    if (const FieldDecl *fieldDecl = dyn_cast<FieldDecl>(memberExpr->getMemberDecl())) {
      string fieldName = fieldDecl->getNameAsString();

      if (oriText.find(fieldName) == string::npos) 
        return;

      QualType fieldType = fieldDecl->getType();

      #if MUT46_OUTPUT
      llvm::errs() << "MUT46: " << "Match int field usage: [" << stringutils::rangetoStr(SM, memberExpr->getSourceRange()) 
                    << "] of [" << fieldName << "]" << "\n";
      #endif

      assert(exprType->isIntegerType() && fieldType->isIntegerType() 
              && "[MUTATION ASSERT] Mut46: declaration and usage should both be int.\n");
      
      SourceRange fieldRange = fieldDecl->getSourceRange();
      string fieldRangeStr = fieldRange.printToString(SM);
      if (viewed.find(fieldRangeStr) == viewed.end()) {
        assert(fieldName != "" && "[MUTATION ASSERT] Mut46: should have field name.\n");
        string repText = "/*mut46*/int " + fieldName + "[10]={0}";
        Rewrite.ReplaceText(fieldRange, repText);

        viewed.insert(fieldRangeStr);
      }

      string repText = "/*mut46*/" + oriText + "[0]";
      Rewrite.ReplaceText(exprRange, repText);
    }
  }
}

void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr(hasType(isInteger()), hasDeclaration(fieldDecl())).bind("intMemberUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDeclWithCtor")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    string recordName = recordDecl->getNameAsString();

    #if MUT47_OUTPUT
    llvm::errs() << "MUT47: " << "Match record with constructor: " << recordName << "\n";
    #endif

    string insText = "\n/*mut47*/\n";
    bool insFlag = false;

    for (const CXXConstructorDecl* ctor : recordDecl->ctors()) {
      bool allBasicFlag = true;
      string insParmText = "";
      
      for (const ParmVarDecl *parm : ctor->parameters()) {
        SourceRange typeRange(parm->getTypeSpecStartLoc(), parm->getTypeSpecEndLoc());
        string typeStr = stringutils::rangetoStr(SM, typeRange);
        if (basicTypeValMap.find(typeStr) == basicTypeValMap.end()) {
          allBasicFlag = false;
          break;
        }
        else {
          if (insParmText != "")
            insParmText += ", ";
          insParmText += basicTypeValMap.at(typeStr);
        }
      }

      if (allBasicFlag && insParmText != "") {
        #if MUT47_OUTPUT
        llvm::errs() << "\t" << "find all basic type ctor: " << stringutils::rangetoStr(SM, ctor->getSourceRange()) << "\n";
        #endif

        insText += "\t" + recordName + "() : " + recordName + "{" + insParmText + "} {}\n";
        insFlag = true;
      }
    }

    if (insFlag) {
      SourceLocation recordEndLoc = recordDecl->getEndLoc();
      Rewrite.InsertText(recordEndLoc, insText, false, true);

      #if MUT47_OUTPUT
      llvm::errs() << "\t" << "insert: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxConstructorDecl())).bind("recordDeclWithCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDeclWithCtor")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    string recordName = recordDecl->getNameAsString();

    if (recordCtorPramMap.find(recordName) != recordCtorPramMap.end()) {
      return;
    }

    #if MUT51_OUTPUT
    llvm::errs() << "MUT51: " << "Match record with constructor: " << recordName << "\n";
    #endif

    for (const CXXConstructorDecl* ctor : recordDecl->ctors()) {
      AccessSpecifier as = ctor->getAccess();
      if (as != AS_public) {
        continue;
      }

      bool allBasicFlag = true;
      string insParmText = "";
      
      for (const ParmVarDecl *parm : ctor->parameters()) {
        SourceRange typeRange(parm->getTypeSpecStartLoc(), parm->getTypeSpecEndLoc());
        string typeStr = stringutils::rangetoStr(SM, typeRange);
        if (basicTypeValMap.find(typeStr) == basicTypeValMap.end()) {
          allBasicFlag = false;
          break;
        }
        else {
          if (insParmText != "")
            insParmText += ", ";
          insParmText += basicTypeValMap.at(typeStr);
        }
      }

      if (allBasicFlag && insParmText != "") {
        #if MUT51_OUTPUT
        llvm::errs() << "\t" << "find all basic type ctor: " << stringutils::rangetoStr(SM, ctor->getSourceRange()) << "\n";
        #endif

        recordCtorPramMap[recordName].push_back(insParmText);
      }
    }
  }
  else if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    string recordName = recordDecl->getNameAsString();

    if (recordCtorPramMap.size() == 0) {
      return;
    }
    if (recordCtorPramMap.find(recordName) != recordCtorPramMap.end()) {
      return;
    }

    for (const CXXConstructorDecl* ctor : recordDecl->ctors()) {
      if (ctor->getNumParams() == 0) {
        return;
      }
    }

    if (inserted.find(recordName) != inserted.end()) {
      return;
    }
    else {
      inserted.insert(recordName);
    }

    string insText = "";
    string initList = "";
    
    for (const auto& pair : recordCtorPramMap) {
      int idx = 0;
      string rName = pair.first;
      vector<string> pramList = pair.second;
      string tmp = "";
      for (const auto& pram : pramList) {
        string varName = recordName + "_" + rName + "_" + to_string(idx++);
        if (tmp != "")
          tmp += ", ";
        tmp += varName;

        if (initList != "")
          initList += ", ";
        initList += varName + "(" + pram + ")";
      }

      tmp = "\t" + rName + " " + tmp + ";\n";
      if (insText == "")
        insText += "\n/*mut51*/\n";
      insText += tmp;
    }

    string ctor = "\t" + recordName + "() : " + initList + " {}\n";
    insText += ctor;
    SourceLocation recordEndLoc = recordDecl->getEndLoc();
    Rewrite.InsertText(recordEndLoc, insText, false, true);

    #if MUT51_OUTPUT
    llvm::errs() << "\t" << "insert: " << insText << "\n";
    #endif
  }
}

void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = cxxRecordDecl(has(cxxConstructorDecl())).bind("recordDeclWithCtor");
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}
