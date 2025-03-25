#include "../include/Inheritance.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto bases = DL->getDefinition()->bases();
    for (auto base : bases) {
      auto base_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              base.getSourceRange());
      auto length = base_str.length();
      // llvm::outs()<<base_str<<'\n';
      bool changeToPrivate = false;
      if (base_str.find("private", 0) != string::npos) {
        stringutils::strReplace(base_str, "private", "public");
      } else if (base_str.find("public", 0) != string::npos) {
        stringutils::strReplace(base_str, "public", "private");
        changeToPrivate = true;
      } else {
        base_str = "private " + base_str;
        changeToPrivate = true;
      }
      base_str = "/*mut1*/" + base_str;
      llvm::outs() << base_str << '\n';
      Rewrite.ReplaceText(base.getBeginLoc(), length, base_str);

      // well-formed validate
      if (changeToPrivate) {
        auto changedBaseName = base.getType()->getAsCXXRecordDecl()->getNameAsString();
        llvm::errs() << "MUT1: " << "changeToPrivate " << DL->getNameAsString() << " : " << changedBaseName << "\n";
        for (auto derivedCls : getDerivedClasses(DL)) {
          llvm::errs() << "\t" << "derivedCls: " << derivedCls->getNameAsString() << "\n";
          for (auto field : derivedCls->fields()) {
            auto fieldTypeName = field->getType().getAsString();
            QualType fieldType = field->getType();

            if (const RecordType *recordType = fieldType->getAs<RecordType>()) {
                const RecordDecl *recordDecl = recordType->getDecl();
                auto qualifiedName = recordDecl->getQualifiedNameAsString();
                llvm::outs() << "\t\tField '" << field->getNameAsString()
                            << "' has type '" << qualifiedName << "'\n";
                if (qualifiedName.find(changedBaseName + "::") == 0) {
                  llvm::errs() << "\t\t" << "invalid field: " << qualifiedName << "\n";
                  WELL_FORMED_INFO = -1;
                  break;
                }
            }
            
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    Context.getTranslationUnitDecl()->dump();
    // well-formed validate
    getInheritanceMap(Context, TheRewriter);
    dumpInheritanceMap();

    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    // L->dump();
    auto TD = CL->getDestructor();
    if (TD == nullptr)
      return;
    auto def =
        stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
    string dtor_definition = "~" + def + "()=default;\n";
    dtor_definition = "/*mut2*/" + dtor_definition;
    if (TD->isImplicit()) {
      llvm::outs() << dtor_definition << '\n';
      Rewrite.ReplaceText(CL->getEndLoc(), 0, dtor_definition);
    }
  }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
  string acc_sep;
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    cur_classes.push_back(CL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
        // auto usingdecl=(UsingDecl)decl;
      }
    }
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      if (DL->isDerivedFrom(cur_classes[j])) {
        auto fields = cur_classes[j]->fields();
        std::vector<FieldDecl *> public_field;
        std::vector<FieldDecl *> protected_field;
        for (auto field : fields) {
          if (field->getAccess() == AccessSpecifier::AS_protected)
            protected_field.push_back(field);
          else if (field->getAccess() == AccessSpecifier::AS_public)
            public_field.push_back(field);
        }
        if (!protected_field.empty()) {
          acc_sep += "\npublic:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : protected_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
          llvm::outs() << acc_sep;
          // auto Err =
          // rep.add(Replacement(*(Result.SourceManager),DL->getEndLoc(),0,acc_sep));
        }
        if (!public_field.empty()) {
          acc_sep += "protected:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : public_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
        }
      }
    }
    acc_sep = "\n/*mut3*/" + acc_sep;
    llvm::outs() << acc_sep;
    Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
  }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
  SourceManager &SM = Rewrite.getSourceMgr();
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
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->hasInheritedConstructor()) {
      llvm::outs() << DL->getNameAsString() << " already exitst\n";
      return;
    }
    std::vector<const clang::CXXRecordDecl *> insertedClasses;
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      // auto using_decls=DL->using
      if (DL->isDerivedFrom(cur_classes[j])) {
        acc_sep = "\npublic:\n";
        acc_sep += "using " + cur_classes[j]->getNameAsString() +
                   ":: " + cur_classes[j]->getNameAsString() + ";\n";
        insertedClasses.push_back(cur_classes[j]);
      }
    }
    acc_sep = "\n/*mut4*/" + acc_sep;
    llvm::outs() << acc_sep;
    Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);

    // well-formed validate
    if (DL->hasUserDeclaredConstructor()) {
      llvm::errs() << "MUT4: " << "hasUserDeclaredConstructor" << "\n";

      for (auto cls : insertedClasses) {
        llvm::errs() << "MUT4: " << "has insertedClasses of " << cls->getNameAsString() << "\n";

        for (auto insteredCtor : cls->ctors()) {
          for (auto oriCtor : DL->ctors()) {
            llvm::errs() << "\t" << "insteredCtor: " << insteredCtor->getNameAsString() << " oriCtor: " << oriCtor->getNameAsString() << "\n";
            if (insteredCtor->getNumParams() == oriCtor->getNumParams()) {
              bool isSame = true;
              for (int i = 0; i < insteredCtor->getNumParams(); i++) {
                QualType T1 = insteredCtor->getParamDecl(i)->getType().getNonReferenceType().getCanonicalType().getUnqualifiedType();
                QualType T2 = oriCtor->getParamDecl(i)->getType().getNonReferenceType().getCanonicalType().getUnqualifiedType();
                llvm::errs() << "\t\t" << "T1: " << T1.getAsString() << " T2: " << T2.getAsString() << " " << Result.Context->hasSameType(T1, T2) << "\n";
                if (!Result.Context->hasSameType(T1, T2)) {
                  isSame = false;
                  break;
                }
              }
              if (isSame) {
                llvm::errs() << "\t" << "has same constructor" << "\n";
                WELL_FORMED_INFO = -1;
                break;
              }
            }
          }
        }
      }
    }
    else {
      llvm::errs() << "MUT4: " << "not hasUserDeclaredConstructor" << "\n";
      
      if (hasInstanceOfClass(DL)) {
        llvm::errs() << "\t" << "has instance of " << DL->getNameAsString() << "\n";
        WELL_FORMED_INFO = -1;
      }

      if (getDerivedClasses(DL).size() > 0) {
        llvm::errs() << "\t" << "has derived class" << "\n";
        WELL_FORMED_INFO = -1;
      }
    }
  }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
  Context.getTranslationUnitDecl()->dump();
  // well-formed validate
  getAllClassInstance(Context, TheRewriter);
  getInheritanceMap(Context, TheRewriter);
  // dumpInheritanceMap();

  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
  auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(derived_matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Inderived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    // DL->dump();

    if (DL->isCompleteDefinition() == false)
      return;
    
    // llvm::outs()<<DL->getNameAsString()<<'\n';
    auto definition = stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto newdefinition = definition.insert(definition.find("{"), ": /*mut7*/public mut7_empty");

    if (targets == 0) {
      newdefinition = "\nstruct mut7_empty{\n\n};\n" + newdefinition;
    }
    targets++;

    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), newdefinition);
  } else if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                 "Templated")) {
    if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TP->getLocation()))
      return;
    if (targets == 0) {
      llvm::outs() << TP->getNameAsString() << '\n';
      targets = 1;
      Rewrite.ReplaceText(TP->getBeginLoc(), 0, "\nstruct mut7_empty{\n\n};\n");
    }
  }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(unless(hasAnyBase(hasType(cxxRecordDecl())))).bind("Inderived");
    auto template_matcher=classTemplateDecl().bind("Templated");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    // DL->dump();
    if (DL->isCompleteDefinition() == false)
      return;

    #if MUT18_OUTPUT
    llvm::errs() << "MUT18: " << "Match derived class: " << DL->getNameAsString() << "\n";
    #endif

    auto bases = DL->getDefinition()->bases();
    bool flag = false;
    for (auto base : bases) {
      auto base_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              base.getSourceRange());
      auto length = base_str.length();
      if (base_str.find("virtual") == string::npos) {
        base_str = "/*mut18*/virtual " + base_str;
        flag = true;

        #if MUT18_OUTPUT
        llvm::errs() << "\t" << "replace: " << base_str << "\n";
        #endif
      }
      Rewrite.ReplaceText(base.getBeginLoc(), length, base_str);
    }

    // well-formed validate
    if (flag) {
      auto ctors = DL->ctors();
      for (auto ctor : ctors) {
        if (ctor->isConstexpr()) {
          WELL_FORMED_INFO = -1;
        }
      }
    }
  }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
  // SourceManager &SM = *(Result.SourceManager);
  SourceManager &SM = Rewrite.getSourceMgr();
  if (const CXXRecordDecl *classDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("derivedClass")) {
    if (!classDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(classDecl->getBeginLoc())) 
      return;
    if ((!classDecl->isStruct() && !classDecl->isClass()) || classDecl->isLambda())
      return;
    if (!classDecl->isCompleteDefinition())
      return;

    // if (classDecl->isTemplated())
    //   return;

    auto className = classDecl->getNameAsString();

    #if MUT38_OUTPUT
    llvm::errs() << "MUT38: " << "Match derived class: " << className << "\n";
    #endif

    string insText = "/*mut38*/\n";
    bool insFlag = false;
    for (const auto &base : classDecl->bases()) {
      if (const CXXRecordDecl *baseDecl = base.getType()->getAsCXXRecordDecl()) {
        if (baseDecl->isTemplated()) {
          continue;
        }
        auto baseDeclName = baseDecl->getNameAsString();
        string midClassName = "mut38_between_" + baseDeclName + "_" + className + stringutils::getTimestamp();
        string accessSepc_m = getrandom::getRandomAccessSepcifier();
        insText += "class " + midClassName + " : " + accessSepc_m + " " + baseDeclName + "{};\n";
        insFlag = true;

        const CXXBaseSpecifier *baseSpecifier = &base;
        SourceLocation baseSpecStartLoc = baseSpecifier->getBeginLoc();
        SourceLocation baseSpecEndLoc = baseSpecifier->getEndLoc();
        SourceRange baseSpecRange = SourceRange(baseSpecStartLoc, baseSpecEndLoc);

        string oriText = Rewrite.getRewrittenText(baseSpecRange);
        string accessSepc_d = getrandom::getRandomAccessSepcifier();
        std::string ReplacementText = "/*mut38*/" + accessSepc_d + " " + midClassName;
        
        Rewrite.ReplaceText(baseSpecRange, ReplacementText);

        #if MUT38_OUTPUT
        llvm::errs() << "\t" << "replace: " << ReplacementText << "\n";
        #endif
      }
    }
    if (insFlag) {
      SourceLocation classStartLoc = classDecl->getBeginLoc();
      Rewrite.InsertText(classStartLoc, insText, true, true);

      #if MUT38_OUTPUT
      llvm::errs() << "\t" << "insert: " << insText << "\n";
      #endif
    }
  }
}

void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("derivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("derivedClass")) {
      if (!recordDecl)
        return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
        return;
      if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
        return;
      if (!recordDecl->isCompleteDefinition())
        return;

      // if (classDecl->isTemplated())
      //   return;

      auto recordName = recordDecl->getNameAsString();

      #if MUT39_OUTPUT
      llvm::errs() << "MUT39: " << "Match derived record: " << recordName << "\n";
      #endif

      string accessSepc_ctor = getrandom::getRandomAccessSepcifier();
      if (accessSepc_ctor != "")
        accessSepc_ctor += ":";
      string insText = "/*mut39*/" + accessSepc_ctor + "\n";
      bool insFlag = false;
      string paramList = "";
      int paramIdx = 0;
      for (const auto &base : recordDecl->bases()) {
        if (const CXXRecordDecl *baseDecl = base.getType()->getAsCXXRecordDecl()) {
          if (baseDecl->isTemplated()) {
            continue;
          }
          insFlag = true;
          auto baseDeclName = baseDecl->getNameAsString();

          string paramType = baseDeclName;
          string paramName = "mut39_param" + to_string(++paramIdx) + "_" + paramType;

          if (paramList != "")
            paramList += ", ";
          paramList += paramType + " " + paramName;
        }
      }
      if (insFlag) {
        insText += recordName + "(" + paramList + "){}\n";

        SourceLocation insLoc = recordDecl->getEndLoc();
        Rewrite.InsertText(insLoc, insText, false, true);

        #if MUT39_OUTPUT
        llvm::errs() << "\t" << "insert: " << insText << "\n";
        #endif
      }
    }
  }

void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("derivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

/* =========================================== mut 54 =========================================== */
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl1")) {
      if (!recordDecl)
        return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
        return;
      if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
        return;
      if (!recordDecl->isCompleteDefinition())
        return;

      // if (classDecl->isTemplated())
      //   return;

      auto recordName = recordDecl->getNameAsString();
      if (recordName == "")
        return;

      #if MUT54_OUTPUT
      llvm::errs() << "MUT54: " << "Match record: " << recordName << "\n";
      #endif

      if (visitedRecord.find(recordName) == visitedRecord.end()) {
        visitedRecord.insert(recordName);
        lastRecordName = recordName;
        llvm::errs() <<"last record: "<< lastRecordName <<    "\n";
      }
    }
    else if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl2")) {
      if (!recordDecl)
        return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
        return;
      if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
        return;
      if (!recordDecl->isCompleteDefinition())
        return;
      
      // if (classDecl->isTemplated())
      //   return;

      if (lastRecordName == "")
        return;

      auto recordName = recordDecl->getNameAsString();
      if (recordName != lastRecordName)
        return;

      #if MUT54_OUTPUT
      llvm::errs() << "MUT54: " << "Insert last record: " << recordName << "\n";
      #endif

      string insParam = "";
      for (string rn : visitedRecord) {
        insParam += (insParam == "") ? ": " : ", ";
        string as = getrandom::getRandomAccessSepcifier();
        insParam += as + " " + rn;
      }

      string insText = "class Mut_54" + insParam + " {}\n";
      SourceLocation insLoc = recordDecl->getEndLoc();
      Rewrite.InsertTextAfterToken(insLoc, ";\n/*mut54*/"+insText);
    }
  }

void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = cxxRecordDecl().bind("recordDecl1");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.matchAST(Context);

    MatchFinder finder2;
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("recordDecl2");
    finder2.addMatcher(matcher2, &callback);
    finder2.matchAST(Context);
}

