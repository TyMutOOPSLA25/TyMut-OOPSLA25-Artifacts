#include "../include/Template.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClasses")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    auto params = TP->getTemplateParameters();
    auto paralist = stringutils::rangetoStr(*(Result.SourceManager),
                                            params->getSourceRange());
    int judge = getrandom::getRandomIndex(2);
    llvm::outs() << judge + 1 << '\n';
    // Regular
    if (judge == 1 || judge == 0) {
      paralist.insert(paralist.rfind('>'), ",/*mut12*/typename NOS=int");
    }
    // Non type
    else if (judge == 2) {
      paralist.insert(paralist.rfind('>'), ",/*mut12*/ int NON=0");

      // well-formed validate
      if (const CXXRecordDecl *Record = TP->getTemplatedDecl()) {
        if (Record->hasUserDeclaredConstructor()) {
          llvm::errs() << "MUT12: " << "hasUserDeclaredConstructor" << "\n";
          WELL_FORMED_INFO = -1;
        }
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(params->getSourceRange()), paralist);
  }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (classnames.count(DL->getNameAsString()))
      return;
    classnames.insert(DL->getNameAsString());

    Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut22*/template <typename=int>");
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "Templated")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    classnames.insert(TP->getNameAsString());
  }
}

void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto template_matcher = classTemplateDecl().bind("Templated");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
  auto handle = [&](const clang::Decl *DEC) {
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DEC->getSourceRange());
    for (auto classname : classnames) {
      if (content.find(classname + "<") != string::npos) {
        string alias_ref = stringutils::strSlice(
            content, content.find(classname + "<"), content.find(">"));
        llvm::outs() << alias_ref << '\n';
        string alias = "MUT_" + std::to_string(cnt);
        cnt++;
        // string rep=alias+alias_ref+";\n";
        stringutils::strReplace(content, alias_ref, alias);
        content =
            "\n/*mut23*/using " + alias + " = " + alias_ref + ";\n" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DEC->getSourceRange()), content);
      }
    }
  };
  if (auto *TP =
          Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templated")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    classnames.push_back(TP->getNameAsString());
  } else if (auto *DEC = Result.Nodes.getNodeAs<clang::Decl>("Decl")) {
    if (!DEC || !Result.Context->getSourceManager().isWrittenInMainFile(
                    DEC->getLocation()))
      return;
    handle(DEC);
  }
}

void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto decl_matcher = decl(anyOf(fieldDecl(),cxxMethodDecl(),varDecl())).bind("Decl");
    auto template_matcher = classTemplateDecl().bind("Templated");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
          "classTemplateDecl")) {
    #if MUT31_OUTPUT
    llvm::errs() << "MUT31: " << "Match ClassTemplateDecl: "
                 << TP->getNameAsString() << "\n";
    #endif

    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;

    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
    auto mut_ins = "\n/*mut31*/static int mut31_static_int_var;\n";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), mut_ins);

    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TP->getSourceRange()), content);

    #if MUT31_OUTPUT
    llvm::errs() << "MUT31: " << "Instrument ClassTemplateDecl: " << mut_ins
                 << "\n";
    #endif
  }
}

void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("classTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const ClassTemplateDecl *templateClass = Result.Nodes.getNodeAs<ClassTemplateDecl>("templateClass")) {
    if (!templateClass)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(templateClass->getBeginLoc())) 
      return;

    // if (!templateClass->isCompleteDefinition())
    //   return;

    auto templateClassName = templateClass->getNameAsString();

    #if MUT43_OUTPUT
    llvm::errs() << "MUT43: " << "Match template class: " << templateClassName << "\n";
    #endif
    
    string accessSepc = getrandom::getRandomAccessSepcifier();
    accessSepc += accessSepc == "" ? "" : ":\n";
    string insText = "/*mut43*/\n" + accessSepc;
    insText += "\ttemplate <typename ... T> " + templateClassName + "<T... t>(){}\n";

    SourceLocation endLoc = templateClass->getEndLoc();
    Rewrite.InsertText(endLoc, insText, false, true);

    #if MUT43_OUTPUT
    llvm::errs() << "\t" << "insert: " << insText << "\n";
    #endif
  }
}

void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("templateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const auto *call = Result.Nodes.getNodeAs<CallExpr>("templateFuncCall")) {
    if (!call)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(call->getBeginLoc())) 
      return;

    if (const auto *Func = dyn_cast<FunctionDecl>(call->getCalleeDecl())) {
    if (auto templateFunc = Func->getPrimaryTemplate()) {
      if (!Func->isFunctionTemplateSpecialization())
        return;
      if (templateFunc->isImplicit())
        return;
      if (Func->getNumParams() != 0)
        return;

      string funcName = Func->getNameAsString();

      if (inserted.find(funcName) != inserted.end())
        return;
      else
        inserted.insert(funcName);

      bool allBasicFlag = false;
      string argText = "";
      if (auto info = Func->getTemplateSpecializationInfo()) {
        if (auto args = info-> TemplateArguments) {
          for (unsigned i = 0; i < args->size(); ++i) {
            const TemplateArgument &arg = args->get(i);
            if (arg.getKind() == TemplateArgument::Type) {
              auto typeArg = arg.getAsType();
              string typeStr = typeArg.getAsString();
              if (basicTypeValMap.find(typeStr) == basicTypeValMap.end()) {
                allBasicFlag = false;
                break;
              } else {
                allBasicFlag = true;
                if (argText != "")
                  argText += ", ";
                argText += basicTypeValMap.at(typeStr);
              }
            }
            else {
              allBasicFlag = false;
              break;
            }
          }
        }
      }

      if (!allBasicFlag)
        return;

      #if MUT52_OUTPUT
      llvm::errs() << "mut52: " << "found all basic template func call: " << stringutils::rangetoStr(SM, call->getSourceRange()) << "\n";
      #endif

      string repText = funcName + "(" + argText +")";
      Rewrite.ReplaceText(call->getSourceRange(), "/*mut52*/" + repText);

      #if MUT52_OUTPUT
      llvm::errs() << "\t" << "replace call with: " << repText << "\n";
      #endif

      string paramText = "";
      int idx = 0;
      auto paramList = templateFunc->getTemplateParameters();
      for (unsigned i = 0; i < paramList->size(); ++i) {
        const auto &param = paramList->getParam(i);
        string paramName = param->getNameAsString();
        if (paramText != "")
          paramText += ", ";
        paramText += paramName + " p_" + to_string(idx++);
      }
      
      SourceLocation insertLocation = templateFunc->getLocation();
      SourceLocation paramLoc = Lexer::findLocationAfterToken(
        insertLocation, tok::l_paren, Rewrite.getSourceMgr(), Rewrite.getLangOpts(), false);

      if (paramLoc.isValid()) {
        // copy
        string oriText = stringutils::rangetoStr(SM, templateFunc->getSourceRange());
        oriText = "\n" + oriText;
        SourceLocation oriEnd = templateFunc->getEndLoc();
        Rewrite.InsertTextAfterToken(oriEnd, oriText);

        // insert
        Rewrite.InsertText(paramLoc, "/*mut52*/" + paramText, true, true);

        #if MUT52_OUTPUT
        llvm::errs() << "\t" << "add parameter: " << paramText << "\n";
        #endif
      }
    }
    }
  }
}

void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(hasDeclaration(functionDecl(hasParent(functionTemplateDecl())))).bind("templateFuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const auto *varDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
    if (!varDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(varDecl->getBeginLoc())) 
      return;

    const auto ty = varDecl->getType();
    const Type *tyPtr = ty.getTypePtr();
    string tyStr = ty.getAsString();

    if (const auto *ctsd = getCTSD_implicit(tyPtr)) {
      if (!Result.Context->getSourceManager().isWrittenInMainFile(ctsd->getBeginLoc())) 
        return;

      #if MUT53_OUTPUT
      llvm::errs() << "mut53: " << "match template class var: " << stringutils::rangetoStr(SM, varDecl->getSourceRange()) << "\n";
      #endif

      auto chkRes = isAllBasic(ctsd->getTemplateArgs());
      bool allBasicFlag = chkRes.first;
      string argText = chkRes.second;
      if (!allBasicFlag)
        return;

      string varName = varDecl->getNameAsString();
      string repText = tyStr + " " + varName + "{" + argText + "}";

      Rewrite.ReplaceText(varDecl->getSourceRange(), "/*mut53*/" + repText);

      #if MUT53_OUTPUT
      llvm::errs() << "\t" << "replace with: " << repText << "\n";
      #endif

      insertCtor(ctsd);
    }
  }
  else if (const auto *tmpObj = Result.Nodes.getNodeAs<CXXTemporaryObjectExpr>("tmpObj")) {
    if (!tmpObj)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(tmpObj->getBeginLoc())) 
      return;

    const auto ty = tmpObj->getType();
    const Type *tyPtr = ty.getTypePtr();
    string tyStr = ty.getAsString();

    if (const auto *ctsd = getCTSD_implicit(tyPtr)) {
      if (!Result.Context->getSourceManager().isWrittenInMainFile(ctsd->getBeginLoc())) 
        return;
      
      #if MUT53_OUTPUT
      llvm::errs() << "mut53: " << "match template class tmp obj: " << stringutils::rangetoStr(SM, tmpObj->getSourceRange()) << "\n";
      #endif

      auto chkRes = isAllBasic(ctsd->getTemplateArgs());
      bool allBasicFlag = chkRes.first;
      string argText = chkRes.second;
      if (!allBasicFlag)
        return;

      string repText = tyStr + "{" + argText + "}";

      Rewrite.ReplaceText(tmpObj->getSourceRange(), "/*mut53*/" + repText);

      #if MUT53_OUTPUT
      llvm::errs() << "\t" << "replace with: " << repText << "\n";
      #endif

      insertCtor(ctsd);
    }
  }
}

pair<bool, string> MutatorFrontendAction_53::Callback::isAllBasic(
    const TemplateArgumentList &args) {
  bool allBasicFlag = false;
  string argText = "";
  for (unsigned i = 0; i < args.size(); ++i) {
    const TemplateArgument &arg = args.get(i);
    if (arg.getKind() == TemplateArgument::Type) {
      auto typeArg = arg.getAsType();
      string typeStr = typeArg.getAsString();
      if (basicTypeValMap.find(typeStr) == basicTypeValMap.end()) {
        allBasicFlag = false;
        break;
      } else {
        allBasicFlag = true;
        if (argText != "")
          argText += ", ";
        argText += basicTypeValMap.at(typeStr);
      }
    } else {
      allBasicFlag = false;
      break;
    }
  }
  return make_pair(allBasicFlag, argText);
}

void MutatorFrontendAction_53::Callback::insertCtor(const ClassTemplateSpecializationDecl *ctsd) {
  string name = ctsd->getNameAsString();
  if (inserted.find(name) != inserted.end())
    return;
  else
    inserted.insert(name);

  const auto *td = ctsd->getSpecializedTemplate();
  assert(td && "no template class");

  string paramText = "";
  int idx = 0;

  auto paramList = td->getTemplateParameters();
  string paramSign = "";
  for (unsigned i = 0; i < paramList->size(); ++i) {
    const auto &param = paramList->getParam(i);
    string paramName = param->getNameAsString();
    paramSign += paramName;
    if (paramText != "")
      paramText += ", ";
    paramText += paramName + " p_" + to_string(idx++);
  }

  const CXXRecordDecl *recordDecl = td->getTemplatedDecl();
  if (recordDecl) {
    bool flag = false;
    for (const auto *decl : recordDecl->ctors()) {
      if (const CXXConstructorDecl *ctor = dyn_cast<CXXConstructorDecl>(decl)) {
        string sign = "";
        for (const auto &param : ctor->parameters()) {
          sign += param->getType().getAsString();
        }
        if (sign == paramSign) {
          flag = true;
          break;
        }
      }
    }
    if (!flag) {
      string insText = "\npublic:\n";
      insText += name + "(" + paramText + ") {}\n";

      Rewrite.InsertText(recordDecl->getEndLoc(), "/*mut53*/" + insText, false,
                         true);
      #if MUT53_OUTPUT
      llvm::errs() << "\t" << "insert ctor: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher1 = varDecl(
        hasType(classTemplateSpecializationDecl()),
        unless(hasDescendant(
          cxxConstructExpr(unless(argumentCountIs(0)))
        ))
      ).bind("varDecl");
    auto matcher2 = cxxTemporaryObjectExpr(
        hasType(classTemplateSpecializationDecl()),
        unless(hasDescendant(stmt()))
      ).bind("tmpObj");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}
