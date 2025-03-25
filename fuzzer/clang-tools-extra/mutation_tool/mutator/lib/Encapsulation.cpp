#include "../include/Encapsulation.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    // if (cur_classnames.count(DL->getNameAsString()))
    //   return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition()) {
      if (!cur_classes.empty()) {
        // llvm::outs()<<DL->getNameAsString()<<'\n';
        auto index = getrandom::getRandomIndex(cur_classes.size() - 1);
        // llvm::outs()<<index<<'\n';
        auto target_class = cur_classes[index];
        // target_class->dump();
        string friend_dec =
            "friend class " + target_class->getNameAsString() + ";";
        friend_dec = "/*mut6*/ " + friend_dec;
        Rewrite.ReplaceText(DL->getEndLoc(), 0, friend_dec);

        #if MUT6_OUTPUT
        llvm::errs() << "MUT6: " << "Add friend class [" << target_class->getNameAsString() << "] to class [" << DL->getNameAsString() << "]" << "\n";
        #endif

        // well-formed validate
        if (target_class->getDescribedClassTemplate()) {
          #if MUT6_OUTPUT
          llvm::errs() << "MUT6: " << "Added template friend class: " << target_class->getNameAsString() << "\n";
          #endif
          WELL_FORMED_INFO = -1;
        }

        llvm::errs() << "cur_classes size: " << cur_classes.size() << "\n";
        llvm::errs() << "index: " << index << "\n";
        for (int i=0; i < cur_classes.size(); i++) {
          if (i == index) continue;
          llvm::errs() << cur_classes[i]->getNameAsString() << "\n";
          if (cur_classes[i]->getNameAsString() == target_class->getNameAsString()) {
            #if MUT6_OUTPUT
            llvm::errs() << "MUT6: " << "Added duplicate friend class: " << target_class->getNameAsString() << "\n";
            #endif
            WELL_FORMED_INFO = -1;
          }
        }
      }
    }
    cur_classes.push_back(DL);
    // cur_classnames.insert(DL->getNameAsString());
  }
}

void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;
    // DL->dump();
    if (MT->hasBody() && !MT->isVirtual()) {
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      // llvm::outs()<<declaration<<'\n';
      declaration.insert(declaration.find("{") + 1, "\n/*mut9*/[&](){\n");
      declaration.insert(declaration.rfind("}"), "\n}();\n");
      int choice = getrandom::getRandomIndex(1);
      llvm::outs() << choice << '\n';
      if (choice) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
  }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (classnames.count(DL->getNameAsString()))
      return;
    classnames.insert(DL->getNameAsString());
    if (DL->isCompleteDefinition()) {
      if (first == 0) {
        Rewrite.ReplaceText(DL->getInnerLocStart(), 0, "enum class mut_17{\n};\n");
        enumerate_set.push_back("mut_17");
        first = first + 1;
      }
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find("using enum") == string::npos) {
        int judge = getrandom::getRandomIndex(enumerate_set.size() - 1);
        llvm::outs() << enumerate_set[judge] << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut17*/using enum " + enumerate_set[judge] + " ;\n");
      }
    }
  } else if (auto *EN = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!EN || !Result.Context->getSourceManager().isWrittenInMainFile(
                   EN->getLocation()))
      return;
    // DL->dump();
    enumerate_set.push_back(EN->getNameAsString());

  } else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "Templated")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    if (first == 0) {
      llvm::outs() << TP->getNameAsString() << '\n';
      classnames.insert(TP->getNameAsString());
      first = 1;
      Rewrite.ReplaceText(TP->getBeginLoc(), 0, "enum class mut_17{\n};\n");
    }
  }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto enum_matcher = enumDecl().bind("Enums");
    auto template_matcher = classTemplateDecl().bind("Templated");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {

  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (cur_classnames.count(DL->getNameAsString()))
      return;
    if (DL->isCompleteDefinition()) {
      if (!candidate_methods.empty()) {
        int index = getrandom::getRandomIndex(candidate_methods.size() - 1);
        Rewrite.ReplaceText(DL->getEndLoc(), 0, candidate_methods[index]);

        // well-formed validate
        if (has_extra_spec[index]) {
          WELL_FORMED_INFO = -1;
        }
      }

      #if MUT21_OUTPUT
      llvm::errs() << "MUT21: " << "Match class: " << DL->getNameAsString() << "\n";
      #endif

      for (auto method : DL->methods()) {
        if (method->isImplicit())
          continue;
        auto judge = DL->getNameAsString();
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               method->getSourceRange());
        if (content.find(judge) != string::npos)
          continue;
        auto statement = content.substr(0, content.find('{'));
        // llvm::outs() << statement << '\n';
        if (DL->isTemplateDecl()) {
          judge = judge + "<int>";
        }
        statement.insert(statement.find(method->getNameAsString()),
                         judge + "::");
        statement = "\n/*mut21*/friend " + statement;
        statement += ";\n";
        candidate_methods.push_back(statement);

        #if MUT21_OUTPUT
        llvm::errs() << "\t" << "candidate method: " << method->getNameAsString() << "\n";
        #endif

        // well-formed validate
        auto SpecType = method->getExceptionSpecType();
        int has_extra_spec_flag = 0;
        if (SpecType == EST_BasicNoexcept || SpecType == EST_DependentNoexcept) {
          llvm::errs() << "\t\t" << "use noexcept\n";
          has_extra_spec_flag = 1;
        }
        if (SpecType == EST_Dynamic || SpecType == EST_DynamicNone) {
          llvm::errs() << "\t\t" << "use throw\n";
          has_extra_spec_flag = 1;
        }
        if (method->getTrailingRequiresClause()) {
          llvm::errs() << "\t\t" << "use requires\n";
          has_extra_spec_flag = 1;
        }
        has_extra_spec.push_back(has_extra_spec_flag);
      }
    }
    cur_classnames.insert(DL->getNameAsString());
  }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MC = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("memberCall")) {
    if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MC->getBeginLoc()))
      return;

    if (const MemberExpr *Member =
            dyn_cast<MemberExpr>(MC->getCallee())) { // member call
      if (Member->isArrow())
        return;

      std::string memberName = Member->getMemberDecl()->getNameAsString();
      if (const DeclRefExpr *Object =
              dyn_cast<DeclRefExpr>(Member->getBase()->IgnoreImpCasts())) {
        if (const VarDecl *Var = dyn_cast<VarDecl>(Object->getDecl())) {
          std::string instanceName = Var->getNameAsString();
          std::string varDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                        Var->getSourceRange());
          auto varTypeLoc = Var->getTypeSourceInfo()->getTypeLoc();
          auto varType = stringutils::rangetoStr(*(Result.SourceManager),
                                                 varTypeLoc.getSourceRange());
          if (const CXXRecordDecl *RecordDecl =
                  Var->getType()->getAsCXXRecordDecl()) {
            std::string recordName = RecordDecl->getNameAsString();

            if (instrumentedDeclMap.find(varDecl) ==
                instrumentedDeclMap.end()) {
              auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                     Var->getSourceRange());

              std::string mut_memberCalleeName =
                  "mut29_" + instanceName + "_" + memberName + "_ptr";
              instrumentedDeclMap[varDecl] = mut_memberCalleeName;

              content += "; /*mut29*/auto " + mut_memberCalleeName + " = " +
                         varType + "::" + memberName;

              Rewrite.ReplaceText(CharSourceRange::getTokenRange(Var->getSourceRange()), content);

              #if MUT29_OUTPUT
              llvm::errs() << "MUT29: " << "instrument VarDecl" << content
                           << "\n";
              #endif
            }

            std::string mut_memberCalleeName = instrumentedDeclMap[varDecl];
            std::string callByDot = instanceName + "." + memberName;
            std::string callByMutPtr =
                "/*mut29*/(" + instanceName + ".*" + mut_memberCalleeName + ")";
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MC->getSourceRange());
            assert(content.find(callByDot, 0) != string::npos &&
                   "Mut29: can not find a member call by dot");

            stringutils::strReplace(content, callByDot, callByMutPtr);

            Rewrite.ReplaceText( CharSourceRange::getTokenRange(MC->getSourceRange()), content);

            #if MUT29_OUTPUT
            llvm::errs() << "MUT29: " << "instrument MemberExpr" << content
                         << "\n";
            #endif
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr().bind("memberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MD =
          Result.Nodes.getNodeAs<clang::CXXMethodDecl>("inlineMethodDecl")) {
    if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MD->getBeginLoc()))
      return;
    if (MD->isImplicit())
      return;

    if (MD->isInlineSpecified()) {
      SourceLocation StartLoc = MD->getBeginLoc();
      SourceLocation EndLoc =
          MD->getBody() ? MD->getBody()->getBeginLoc() : MD->getEndLoc();

      SourceRange Range(StartLoc, EndLoc);

      auto content = stringutils::rangetoStr(*(Result.SourceManager), Range);
      size_t InlinePos = content.find("inline");
      if (InlinePos == string::npos)
        return;
      #if MUT30_OUTPUT
      llvm::errs() << "MUT30: " << "Match inline MethodDecl: " << content
                   << "\n";
      #endif

      stringutils::strReplace(content, "inline", " /*mut30*/ ");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Range), content);
    }
  }
}

void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(isInline()).bind("inlineMethodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("hasStaticNonVoidMethod")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;

    auto recordName = recordDecl->getNameAsString();

    #if MUT40_OUTPUT
    llvm::errs() << "MUT40: " << "Match record with static non void method: " << recordName << "\n";
    #endif

    string insText = "/*mut40*/\npublic:\n";
    bool insFlag = false;

    for (const auto *methodDecl : recordDecl->methods()) {
      string methodName = methodDecl->getNameAsString();
      SourceRange returnTypeRange = methodDecl->getReturnTypeSourceRange();
      QualType returnType = methodDecl->getReturnType();
      string returnTypeName = stringutils::rangetoStr(SM, returnTypeRange);
      if (methodDecl->isImplicit())
        continue;
      
      bool isStatic = methodDecl->isStatic();
      bool notReturnsVoid = returnTypeName != "void";

      assert(isStatic && notReturnsVoid && "Mut40: should be a static non void method\n");

      if (methodDecl->getNumParams() > 0)
        continue;
    
      #if MUT40_OUTPUT
      llvm::errs() << "\t" << "Static non void method with 0 param: " << methodName << "\n";
      #endif

      string insMethodType = "void";
      string insMethodName = "mut40_" + methodDecl->getNameAsString() 
                              + "_" + returnTypeName + "_" + stringutils::getTimestamp();
      string insMethodParams = "(" + returnTypeName + "=" + methodName + "()" +")";
      insText += "\t" + insMethodType + " " + insMethodName + insMethodParams + "{}\n";
      insFlag = true;
    }
    if (insFlag) {
      SourceLocation recordEndLoc = recordDecl->getEndLoc();
      Rewrite.InsertText(recordEndLoc, insText, false, true);

      #if MUT40_OUTPUT
      llvm::errs() << "\t" << "insert: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxMethodDecl(
                                    isStaticStorageClass(),
                                    unless(returns(asString("void")))
                                ))).bind("hasStaticNonVoidMethod");;
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("classWithMemberVar")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;

    auto recordName = recordDecl->getNameAsString();

    #if MUT41_OUTPUT
    llvm::errs() << "MUT41: " << "Match record with member var: " << recordName << "\n";
    #endif

    string funcName = "mut41_" + recordName + "_" + stringutils::getTimestamp();
    string insText = "\n/*mut41*/\nvoid " + funcName + "(){\n";
    bool insFlag = false;

    for (const auto *field : recordDecl->fields()) {
      QualType fieldType = field->getType();
      string fieldName = field->getNameAsString();
      if (fieldType->isBuiltinType()) {
        string rhs;
        SourceRange fieldRange = field->getSourceRange();
        string fieldText = stringutils::rangetoStr(SM, fieldRange);
        llvm::outs() << fieldText << "\n";
        if (fieldText.find("int ") != string::npos) {
          rhs = "0";
        }
        else if (fieldText.find("bool ") != string::npos) {
          rhs = "true";
        }
        else if (fieldText.find("char ") != string::npos) {
          rhs = "\'0\'";
        }
        else 
          continue;
        
        insFlag = true;
        
        bool isStatic = (fieldText.find("static ") != string::npos);
        if (isStatic) {
          string lambdaName = "mut41_" + fieldName + "_" + recordName + "_lambda";
          
          insText += "\tauto " + lambdaName + " = [&](){ " + recordName + "::" + fieldName + " = " + rhs + "; };\n\n";
        }
        else {
          string varName = "mut41_" + fieldName + "_" + recordName;
          // insText += "\t" + recordName + " " + varName + " = new " + recordName + "();\n";
          insText += "\t" + recordName + " " + varName + ";\n";

          string lambdaName = varName + "_lambda";
          
          insText += "\tauto " + lambdaName + " = [&](){ " + varName + "." + fieldName + " = " + rhs + "; };\n\n";

        }
      }
    }

    insText += "}\n";

    if (insFlag) {
      SourceLocation recordEndLoc = recordDecl->getEndLoc();
      recordEndLoc = Lexer::findLocationAfterToken(recordEndLoc, tok::semi, *Result.SourceManager, Result.Context->getLangOpts(), false);
      Rewrite.InsertText(recordEndLoc, insText, true, true);

      #if MUT41_OUTPUT
      llvm::errs() << "\t" << "insert: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
                                    has(fieldDecl()),
                                    unless(isTemplateInstantiation())
                                  ).bind("classWithMemberVar");;
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  const auto *memberExpr = Result.Nodes.getNodeAs<MemberExpr>("memberUsage");
  const auto *declRecordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("declRecord");
  const auto *ancestorRecordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("ancestorRecord");
  if (memberExpr && declRecordDecl && ancestorRecordDecl) {
    if (!Result.Context->getSourceManager().isWrittenInMainFile(memberExpr->getBeginLoc())) 
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(declRecordDecl->getBeginLoc())) 
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(ancestorRecordDecl->getBeginLoc())) 
      return;

    if (declRecordDecl != ancestorRecordDecl)
      return;
    
    SourceRange oriRange = memberExpr->getSourceRange();
    string oriText = stringutils::rangetoStr(SM, oriRange);

    string rangeStr = oriRange.printToString(SM);
    if (viewed.find(rangeStr) != viewed.end())
      return;
    viewed.insert(rangeStr);

    #if MUT42_OUTPUT
    llvm::errs() << "MUT42: " << "Match member usage: " << oriText 
                  << " decl in [" << declRecordDecl->getNameAsString()
                  << "] use in [" << ancestorRecordDecl->getNameAsString() <<"]\n";
    // llvm::errs() << rangeStr << "\n";
    #endif


    if (oriText.find("this") == string::npos) {
      oriText = "/*mut42*/this->" + oriText;
      Rewrite.ReplaceText(oriRange, oriText);
    }
  }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = memberExpr(
                                member(fieldDecl(hasParent(cxxRecordDecl().bind("declRecord")))),
                                hasAncestor(cxxRecordDecl().bind("ancestorRecord"))
                              ).bind("memberUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}


