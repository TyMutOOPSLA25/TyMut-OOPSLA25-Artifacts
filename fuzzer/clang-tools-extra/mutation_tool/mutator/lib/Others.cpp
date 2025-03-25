#include "../include/Others.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    // DL->dump();
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    // if(!DL||!Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
    // return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    // llvm::outs()<<content<<'\n';

    int dice = getrandom::getRandomIndex(2);
    std::string ins_type = "";
    if (dice == 0)
      ins_type = "private";
    else if (dice == 1)
      ins_type = "protected";
    else if (dice == 2)
      ins_type = "public";
    else
      assert(false && "Mut13: Dice should have 3 possible values!");

    auto constexrdecl =
        "\n\t/*mut13*/" + ins_type + ":\n\tstatic constexpr int mut_13=0;\n";
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), constexrdecl);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    // FS->printPretty()
    // Replacement touch(*(Result.SourceManager),DL->getInnerLocStart(),0,
    // "touch_2"); Replacement r;

    cur_classes.push_back(DL);
    cur_classes_type.push_back(dice);
  } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>(
                 "Return0Stmt")) { 
    if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   RS->getBeginLoc()))
      return;


    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    if (cur_classes_type[index] != 2) 
      return;
    auto target = cur_classes[index];

    auto ins = "/*mut13*/return " + target->getNameAsString() + "::mut_13";

    Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), ins);
  }
}

void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto return0Stmt_macher =
        returnStmt(hasReturnValue(integerLiteral(equals(0))))
            .bind("Return0Stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(return0Stmt_macher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;
    // DL->dump();
    if (MT->hasBody() && !MT->isVirtual()) {
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("noexcept") != string::npos)
        content.replace(content.find("noexcept"), 8, "/*mut14*/");
      else
        content.insert(content.find('{'), "/*mut14*/noexcept");
      int judge = getrandom::getRandomIndex(1);
      if (judge) {
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
  }
}

void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;
    // DL->dump();
    if (MT->hasBody() && !MT->isVirtual()) {
      auto name = MT->getNameAsString();
      if (template_methods.count(name))
        return;
      auto pasize = MT->parameters().size();
      if (pasize == 0 || pasize == 1) {
        auto overload_0_para = "/*mut15*/template <typename> " +
                               MT->getReturnType().getAsString() + ' ' + name +
                               "(){}";
        auto overload_1_para = "/*mut15*/template <typename MUT> " +
                               MT->getReturnType().getAsString() + ' ' + name +
                               "(MUT t){}";
        llvm::outs() << overload_0_para << '\n';
        Rewrite.ReplaceText(MT->getEndLoc(), 1, "}\n" + overload_0_para + '\n' + overload_1_para);
      }
    }
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                 "Templated Methods")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    llvm::outs() << TP->getNameAsString();
    template_methods.insert(TP->getNameAsString());
  }
}

void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto method_matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    auto template_matcher = functionTemplateDecl(unless(cxxConstructorDecl())).bind("Templated Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    // DL->dump();
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;
    if (MT->isPure()) {
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("= 0") != string::npos)
        stringutils::strReplace(content, "= 0", "/*mut19*/{}");
      else if (content.find("=0") != string::npos)
        stringutils::strReplace(content, "=0", "/*mut19*/{}");
      
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
  }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Records")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    cur_classes.push_back(CL);
  } 
  else if (auto *FC = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
    if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FC->getLocation()))
      return;
    if (FC->isImplicit())
      return;

    if (cur_classes.empty())
      return;

    bool funcMatched = false;
    for (const auto *Param : FC->parameters()) {
      if (Param->getType()->isIntegerType() ||
          Param->getType()->isBooleanType() || Param->getType()->isCharType()) {

        size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target = cur_classes[index];

        if (target->getSourceRange().fullyContains(
                FC->getSourceRange())) { 
          #if MUT26_OUTPUT
          llvm::errs() << "MUT26: " << "Skip function " << FC->getNameAsString()
                       << " in Record " << target->getNameAsString() << "\n";
          #endif
          continue;
        }

        auto ins = "/*mut26*/=" + target->getNameAsString() + "{}";
        auto content = stringutils::rangetoStr(*(Result.SourceManager), Param->getSourceRange());
        content += ins;
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Param->getSourceRange()), content);
        funcMatched = true;
      }
    }

    if (funcMatched) {
      if (!recordsInstrumented) {
        recordsInstrumented = true;

        for (auto record : cur_classes) {
          bool hasIntConversion = false;
          bool hasBoolConversion = false;
          bool hasCharConversion = false;
          for (const auto *Method : record->methods()) {
            if (const CXXConversionDecl *ConversionDecl =
                    dyn_cast<CXXConversionDecl>(Method)) {
              if (ConversionDecl->getReturnType().getAsString() == "int") {
                hasIntConversion = true;
              }
              if (ConversionDecl->getReturnType().getAsString() == "bool") {
                hasBoolConversion = true;
              }
              if (ConversionDecl->getReturnType().getAsString() == "char") {
                hasCharConversion = true;
              }
            }
          }
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 record->getSourceRange());
          if (!hasIntConversion) {
            auto conversion = "\n\t/*mut26*/operator int() {return 0;};";
            if (content.rfind('}') != string::npos)
              content.insert(content.rfind('}'), conversion);
          }
          if (!hasBoolConversion) {
            auto conversion = "\n\t/*mut26*/operator bool() {return 1;};";
            if (content.rfind('}') != string::npos)
              content.insert(content.rfind('}'), conversion);
          }
          if (!hasCharConversion) {
            auto conversion = "\n\t/*mut26*/operator char() {return 'c';};";
            if (content.rfind('}') != string::npos)
              content.insert(content.rfind('}'), conversion);
          }
          content.insert(content.rfind('}'), "\n");

          #if MUT26_OUTPUT
          {
            auto msg = hasIntConversion                     ? ""
                       : "IntConvert " + hasBoolConversion  ? ""
                       : "BoolConvert " + hasCharConversion ? ""
                                                            : "CharConvert ";
            llvm::errs() << "MUT26: " << "Instrument record: "
                         << record->getNameAsString() << " " << msg << "\n"
                         << content << "\n";
          }
          #endif

          Rewrite.ReplaceText(CharSourceRange::getTokenRange(record->getSourceRange()), content);
        }
      }
    }

    #if MUT26_OUTPUT
    if (funcMatched)
      llvm::errs() << "MUT26: " << "Match function: " << FC->getNameAsString()
                   << "\n";
    #endif
  }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto records_matcher = cxxRecordDecl().bind("Records");
    auto func_matcher =
        functionDecl(unless(cxxConversionDecl()), hasBody(compoundStmt()))
            .bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(records_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DC = Result.Nodes.getNodeAs<clang::Stmt>("cast")) {
    if (!DC || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DC->getBeginLoc()))
      return;

    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DC->getSourceRange());

    #if MUT27_OUTPUT
    llvm::errs() << "MUT27: " << "Match dynamic_cast or static_cast: "
                 << content << "\n";
    #endif

    auto mut_content = "/*mut27*/" + content;
    int dice = getrandom::getRandomIndex(1);
    if (dice == 0) { // reinterpret_cast
      if (mut_content.find("dynamic_cast", 0) != string::npos)
        stringutils::strReplace(mut_content, "dynamic_cast",
                                "reinterpret_cast");
      else if (mut_content.find("static_cast", 0) != string::npos)
        stringutils::strReplace(mut_content, "static_cast", "reinterpret_cast");
      else
        assert(false && "Mut27: dont have dynamic_cast or static_cast");
    } else if (dice == 1) { 
      if (mut_content.find("dynamic_cast", 0) != string::npos)
        stringutils::strReplace(mut_content, "dynamic_cast", " ");
      else if (mut_content.find("static_cast", 0) != string::npos)
        stringutils::strReplace(mut_content, "static_cast", " ");
      else
        assert(false && "Mut27: dont have dynamic_cast or static_cast");
      if (mut_content.find("<", 0) != string::npos) {
        stringutils::strReplace(mut_content, "<", "(");
        assert(mut_content.find("<", 0) == string::npos &&
               "Mut27: have multi <");
      } else
        assert(false && "Mut27: dont have <");
      if (mut_content.find(">", 0) != string::npos) {
        stringutils::strReplace(mut_content, ">", ")");
        assert(mut_content.find(">", 0) == string::npos &&
               "Mut27: have multi >");
      } else
        assert(false && "Mut27: dont have >");
    }
    // else if (dice == 2) {   // static_cast
    //     if(mut_content.find("dynamic_cast",0)!=string::npos)
    //         stringutils::strReplace(mut_content,"dynamic_cast","static_cast");
    //     else if (mut_content.find("static_cast",0)!=string::npos)
    //         stringutils::strReplace(mut_content,"static_cast"," ");
    //     else
    //         assert(false && "Mut27: dont have dynamic_cast or static_cast");
    // }
    else {
      assert(false && "Mut27: Dice should have 2 possible values!");
    }

    #if MUT27_OUTPUT
    llvm::errs() << "MUT27: " << dice << "\n\t    " << content << "\n\t => "
                 << mut_content << "\n";
    #endif

    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DC->getSourceRange()), mut_content);
  }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = stmt(anyOf(cxxDynamicCastExpr(), cxxStaticCastExpr())).bind("cast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DD =
          Result.Nodes.getNodeAs<clang::DecompositionDecl>("Decomposition")) {
    if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DD->getBeginLoc()))
      return;

    auto content = stringutils::rangetoStr(*(Result.SourceManager), DD->getSourceRange());
    auto DDType = DD->getType();

    if (DDType->isReferenceType()) {
      #if MUT28_OUTPUT
      llvm::errs() << "MUT28: " << "Match DecompositionDecl: " << content
                   << "\n";
      #endif

      const TypeLoc &TypeLoc = DD->getTypeSourceInfo()->getTypeLoc();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TypeLoc.getSourceRange());

      assert(content.find("&", 0) != string::npos &&
             "Mut28: do not a reference type (no &)");

      stringutils::strReplace(content, "&", "/*mut28*/");

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TypeLoc.getSourceRange()), content);
    } 
    else
      return;
    return;
  }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = decompositionDecl().bind("Decomposition");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
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

    if (recordInsMap.find(recordName) != recordInsMap.end()) {
      return;
    }

    #if MUT48_OUTPUT
    llvm::errs() << "MUT48: " << "Match record with constructor: " << recordName << "\n";
    #endif

    string insText = "";

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
        #if MUT48_OUTPUT
        llvm::errs() << "\t" << "find all basic type ctor: " << stringutils::rangetoStr(SM, ctor->getSourceRange()) << "\n";
        #endif

        string retStmt = "return " + recordName + "{" + insParmText + "};";
        string tmp = "\toperator " + recordName + "() { " + retStmt + " }\n";

        insText += tmp;
      }
    }
    if (insText != "") {
      recordInsMap[recordName] = insText;
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

    if (recordInsMap.find(recordName) != recordInsMap.end()) {
      return;
    }

    if (inserted.find(recordName) != inserted.end()) {
      return;
    }
    else {
      inserted.insert(recordName);
    }

    string insText = "";
    
    for (const auto& pair : recordInsMap) {
      if (insText == "") {
        insText += "\n/*mut48*/\n";
      }
      insText += pair.second;
    }

    if (insText != "") {
      SourceLocation recordEndLoc = recordDecl->getEndLoc();
      Rewrite.InsertText(recordEndLoc, insText, false, true);

      #if MUT48_OUTPUT
      llvm::errs() << "\t" << "insert: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = cxxRecordDecl(has(cxxConstructorDecl())).bind("recordDeclWithCtor");
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordWithFields")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    string recordName = recordDecl->getNameAsString();

    if (recordInsMap.find(recordName) != recordInsMap.end()) {
      return;
    }

    string insText = "";
    
    set<string> insertedTypeSet;
    for (const auto *field : recordDecl->fields()) {

      SourceRange typeRange(field->getTypeSpecStartLoc(), field->getTypeSpecEndLoc());
      string typeStr = stringutils::rangetoStr(SM, typeRange);

      if (basicTypeValMap.find(typeStr) == basicTypeValMap.end()) {
        continue;
      }
      else {
        if (insertedTypeSet.find(typeStr) != insertedTypeSet.end()) {
          continue;
        }
        else {
          insertedTypeSet.insert(typeStr);
          typeRecordsMap[typeStr].push_back(recordName);
          
          if (insText == "") {
            insText += "\n/*mut49*/\npublic:\n";
          }
          string fieldName = field->getNameAsString();
          assert(fieldName != "" && "Mut49: Empty filed name\n");
          string retStmt = "return " + fieldName + ";"; 
          insText += "\toperator " + typeStr + "() { " + retStmt + " }\n";
        }
      }
    }

    if (insText != "") {
      SourceLocation recordEndLoc = recordDecl->getEndLoc();
      recordInsMap[recordName] = make_pair(false, make_pair(recordEndLoc, insText));
    }
    
  }
  else if (const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
    if (!varDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(varDecl->getBeginLoc())) 
      return;
    if (varDecl->hasInit()) {
      VarDecl::InitializationStyle initStyle = varDecl->getInitStyle();
      if (initStyle == VarDecl::InitializationStyle::CallInit) {
        return; 
      }
    }

    string varName = varDecl->getNameAsString();
    SourceRange typeRange(varDecl->getTypeSpecStartLoc(), varDecl->getTypeSpecEndLoc());
    string typeStr = stringutils::rangetoStr(SM, typeRange);

    vector<string> candidates = typeRecordsMap.at(typeStr);
    size_t candidatesSize = candidates.size();
    if (candidatesSize == 0) {
      return;
    }

    size_t idx = getrandom::getRandomIndex(candidatesSize - 1);
    string recordName = candidates[idx];

    if (recordInsMap[recordName].first == false) {
      auto insPair = recordInsMap[recordName].second;
      SourceLocation insLoc = insPair.first;
      string insText = insPair.second;
      Rewrite.InsertText(insLoc, insText, false, true);

      recordInsMap[recordName].first = true;
    }

    string repText = typeStr + " " + varName + " = " + recordName + "{}";

    Rewrite.ReplaceText(varDecl->getSourceRange(), repText);
  }
 
}

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = recordDecl(has(fieldDecl())).bind("recordWithFields");
    DeclarationMatcher matcher2 = varDecl().bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const ClassTemplateDecl *templateClass = Result.Nodes.getNodeAs<ClassTemplateDecl>("templateClass")) {
    if (!templateClass)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(templateClass->getBeginLoc())) 
      return;

    auto templateClassName = templateClass->getNameAsString();

    if (const CXXRecordDecl *classDecl = templateClass->getTemplatedDecl()) {
      for (const auto *method : classDecl->methods()) {
        if (method->getTrailingRequiresClause()) {
          continue;
        }
        else {
          string insText = "/*mut50*/requires true";

          if (auto body = method->getBody()) {
            SourceLocation insLoc = body->getBeginLoc();
            Rewrite.InsertText(insLoc, insText, false, false);
          }
          else {
            SourceLocation startLoc = method->getBeginLoc();
            SourceLocation endLoc = method->getEndLoc();
            SourceRange signRange(startLoc, endLoc);
            string oriSignStr = stringutils::rangetoStr(SM, signRange);
            string repText = oriSignStr + insText;
            Rewrite.ReplaceText(signRange, repText);
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("templateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

