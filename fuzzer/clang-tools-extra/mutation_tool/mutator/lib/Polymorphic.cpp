#include "../include/Polymorphic.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("overrideMethodDecl")) {
    if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MD->getBeginLoc()))
      return;
    if (MD->isImplicit())
      return;

    if (MD->hasAttr<clang::OverrideAttr>()) {
      const clang::OverrideAttr *Attr = MD->getAttr<clang::OverrideAttr>();
      SourceLocation OverrideLoc = Attr->getLocation();
      if (OverrideLoc.isValid()) {

        SourceLocation StartLoc = MD->getBeginLoc();
        SourceLocation EndLoc =
            MD->getBody() ? MD->getBody()->getBeginLoc() : MD->getEndLoc();
        SourceRange signRange(StartLoc, EndLoc);
        auto signContent =
            stringutils::rangetoStr(*(Result.SourceManager), signRange);

        size_t OverridePos = signContent.find("override");
        assert(OverridePos != string::npos && "Mut32: no override keyword");

        #if MUT32_OUTPUT
        llvm::errs() << "MUT32: " << "Match override MethodDecl: "
                     << signContent << "\n";
        #endif

        stringutils::strReplace(signContent, "override", " /*mut32*/ ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(signRange), signContent);

        #if MUT32_OUTPUT
        llvm::errs() << "\t" << "Erase override keyword: " << signContent
                     << "\n";
        #endif
      }
    }
  }
}

void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxMethodDecl(isOverride()).bind("overrideMethodDecl");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *Class = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("cxxRecordDecl")) {
    if (!Class || !Result.Context->getSourceManager().isWrittenInMainFile(
                      Class->getBeginLoc()))
      return;

    #if MUT33_OUTPUT
    llvm::errs() << "MUT33: " << "Match Record with override method: "
                 << Class->getNameAsString() << "\n";
    #endif

    std::map<std::string, const clang::CXXMethodDecl *>
        overrideMethodReturnTypeMap;
    for (const auto *Method : Class->methods()) {
      if (Method->isImplicit())
        continue;
      auto returnType = Method->getReturnType().getAsString();
      if (returnType == "void")
        continue;
      if (Method->getNumParams() != 0)
        continue;
      if (Method->size_overridden_methods() > 0 &&
          Method->isThisDeclarationADefinition()) {
        #if MUT33_OUTPUT
        llvm::errs() << "\t" << "Found override method: " << returnType << " "
                     << Method->getNameAsString() << "\n";
        #endif

        overrideMethodReturnTypeMap[returnType] = Method;
      }
    }

    for (const auto *Ctor : Class->ctors()) {
      if (Ctor->isImplicit())
        continue;
      if (Ctor->getNumParams() != 1)
        continue;

      #if MUT33_OUTPUT
      llvm::errs() << "\t" << "Found constructor with one parameter: "
                   << Class->getNameAsString() << "(";
      for (const auto *Param : Ctor->parameters()) {
        llvm::errs() << Param->getType().getAsString() << " "
                     << Param->getNameAsString();
        if (Param != Ctor->parameters().back()) {
          llvm::errs() << ", ";
        }
      }
      llvm::errs() << ")\n";
      #endif

      assert(Ctor->parameters().size() == 1 && "multi parameters found");
      auto paramType = Ctor->parameters()[0]->getType().getAsString();

      auto it = overrideMethodReturnTypeMap.find(paramType);
      if (it != overrideMethodReturnTypeMap.end())
        overrideMethodReturnTypeMap.erase(it);
    }

    #if MUT33_OUTPUT
    for (const auto &entry : overrideMethodReturnTypeMap) {
      const std::string &returnType = entry.first;
      const clang::CXXMethodDecl *Method = entry.second;

      llvm::errs() << "\t"
                   << "Found override method need instrument: " << returnType
                   << " " << Method->getNameAsString() << "\n";
    }
    #endif

    std::map<std::string, std::string> varMap;
    for (const auto *Field : Class->fields()) {
      llvm::outs() << "\tMember variable: " << Field->getType().getAsString()
                   << " " << Field->getNameAsString() << "\n";
      auto varType = Field->getType().getAsString();
      auto varName = Field->getNameAsString();
      assert(varType != "" && varName != "" &&
             "Member variable no type or no name");
      auto it = overrideMethodReturnTypeMap.find(varType);
      if (it != overrideMethodReturnTypeMap.end())
        varMap[varType] = varName;
    }

    std::string instrumentContent = "";

    bool instrumentFieldFlag = false;
    for (const auto &entry : overrideMethodReturnTypeMap) {
      const std::string &returnType = entry.first;
      if (varMap.find(returnType) == varMap.end()) {
        if (!instrumentFieldFlag) {
          instrumentContent += "\n/*mut33*/\nprivate:\n";
          instrumentFieldFlag = true;
        }

        auto instrumentVarType = returnType;
        auto instrumentVarName = "mut33_" + instrumentVarType;
        instrumentContent +=
            "\t" + instrumentVarType + " " + instrumentVarName + ";\n";
        varMap[instrumentVarType] = instrumentVarName;
      }
    }

    bool instrumentCtorFlag = false;
    auto className = Class->getNameAsString();
    for (const auto &entry : overrideMethodReturnTypeMap) {
      const std::string &paramType = entry.first;
      std::string paramName = varMap[paramType];
      std::string methodName =
          overrideMethodReturnTypeMap[paramType]->getNameAsString();

      if (!instrumentCtorFlag) {
        instrumentContent += "\n/*mut33*/\npublic:\n";
        instrumentCtorFlag = true;
      }

      instrumentContent += "\t" + className + "(" + paramType + " " +
                           paramName + ")" + ":" + paramName + "(" +
                           methodName + "()" + ")" + "{}" + "\n";
    }

    llvm::errs() << instrumentContent << "\n";
    auto classContent = stringutils::rangetoStr(*(Result.SourceManager),
                                                Class->getSourceRange());
    classContent.insert(classContent.rfind('}'), instrumentContent);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(Class->getSourceRange()), classContent);

    #if MUT33_OUTPUT
    llvm::errs() << instrumentContent << "\n";
    #endif
  }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxRecordDecl(
                                has(cxxMethodDecl(isOverride()))
                                ).bind("cxxRecordDecl");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *Record = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("derivedClass")){
    if(!Record||!Result.Context->getSourceManager().isWrittenInMainFile(Record->getBeginLoc())) return;

    for (const auto &BaseSpecifier : Record->bases()) {
      if (const CXXRecordDecl *Base = BaseSpecifier.getType()->getAsCXXRecordDecl()) {
        if (instrumentedClasses.find(Base) != instrumentedClasses.end()) continue;
        
        #if MUT34_OUTPUT
        llvm::errs() << "MUT34: " << "Match base class: " << Base->getNameAsString() << " of class " << Record->getNameAsString() << "\n";                 
        #endif

        const auto *Dtor = Base->getDestructor();
        if (Dtor) {
          auto dtorContent = stringutils::rangetoStr(*(Result.SourceManager), Dtor->getSourceRange());
          #if MUT34_OUTPUT
          llvm::errs() << "\t" << "Found dtor: " << dtorContent << "\n";
          #endif
          continue;
        }
        else {
          #if MUT34_OUTPUT
          llvm::errs() << "\t" << "No dtor" << "\n";
          #endif

          std::string instrumentContent = "";
          instrumentContent += "\n\t/*mut34*/virtual void ~" + Base->getNameAsString() + " = default;\n";

          std::string classContent = stringutils::rangetoStr(*(Result.SourceManager), Base->getSourceRange());
          classContent.insert(classContent.rfind('}'), instrumentContent);
          
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(Base->getSourceRange()), classContent);

          instrumentedClasses.insert(Base);

          #if MUT33_OUTPUT
          llvm::errs() << instrumentContent << "\n";
          #endif
        }
      }
    }
  }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher =  cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("derivedClass");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *Method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("virtualMethod")){
    if(!Method||!Result.Context->getSourceManager().isWrittenInMainFile(Method->getBeginLoc())) return;
    if(Method->isImplicit()) return;

    if (Method->isVirtual() && !Method->hasAttr<FinalAttr>()) {
      auto methodContent = stringutils::rangetoStr(*(Result.SourceManager), Method->getSourceRange());

      assert(methodContent.find("virtual") != string::npos && methodContent.find("final") == string::npos 
              && "This virtual Method must have virtual and must not have final");
      
      SourceLocation StartLoc = Method->getBeginLoc();
      SourceLocation EndLoc = Method->getBody() ? Method->getBody()->getBeginLoc() : Method->getEndLoc();
      SourceRange signRange(StartLoc, EndLoc);
      auto signContent=stringutils::rangetoStr(*(Result.SourceManager), signRange);

      if (Method->getBody()) {
        auto bodyBeginLoc = signContent.rfind('{');
        assert(bodyBeginLoc != string::npos && "Error about signature and body");
        signContent.insert(bodyBeginLoc, " /*mut35*/final ");
      }
      else
        signContent += " /*mut35*/final ";

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(signRange), signContent);

      #if MUT35_OUTPUT
      llvm::errs() << "MUT35: " << "Add final keyword: " << signContent << "\n";                 
      #endif
    }           
  }
}

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxMethodDecl(isVirtual()).bind("virtualMethod");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *Class  = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("cxxRecordWithOverrideMethodDecl_0")){
    if(!Class ||!Result.Context->getSourceManager().isWrittenInMainFile(Class->getBeginLoc())) return;
    
    #if MUT36_OUTPUT
    llvm::errs() << "MUT36: " << "Match Record with override method with no parameters: " << Class->getNameAsString() << "\n";                 
    #endif

    bool instrumentFlag = false;
    std::string varName = "mut36_" + Class->getNameAsString();
    std::string instrumentContent = "\n/*mut36*/void mut36_main_" + Class->getNameAsString() + "_" + stringutils::getTimestamp() + "(){\n" 
                                    + "\t" + Class->getNameAsString() + " " + varName + ";\n"
                                    +"}\n";
    
    for (const auto *Method : Class->methods()) {
      if (Method->isImplicit()) continue;
      if (Method->size_overridden_methods() > 0 && Method->isThisDeclarationADefinition()) {
        #if MUT36_OUTPUT
        llvm::errs() << "\t" << "Found override method: " << Method->getNameAsString() << "\n";
        #endif

        const CXXRecordDecl *CurrentClass = Method->getParent();

        const CXXMethodDecl *OriginalMethod = Method;
        while (!OriginalMethod->overridden_methods().empty()) {
          OriginalMethod = *OriginalMethod->overridden_methods().begin();
        }

        const CXXRecordDecl *OriginalClass = OriginalMethod->getParent();

        if (const auto *TemplateClass1 = llvm::dyn_cast<ClassTemplateSpecializationDecl>(CurrentClass)) {
          continue;
        }
        if (const auto *TemplateClass2 = llvm::dyn_cast<ClassTemplateSpecializationDecl>(OriginalMethod)) {
          continue;
        }

        std::string instrumentContentForMethod = "\tstatic_cast<" + OriginalClass->getNameAsString() + "*>(&" + varName + ")->" + Method->getNameAsString() + "();\n";

        instrumentContent.insert(instrumentContent.rfind("}"), instrumentContentForMethod);

        if (!instrumentFlag) {
          instrumentFlag = true;
        }
      }
    }

    if (instrumentFlag) {
      auto classContent = stringutils::rangetoStr(*(Result.SourceManager), Class->getSourceRange());
      llvm::errs() << classContent << "\n";
      classContent += ";" + instrumentContent;
      llvm::errs() << classContent << "\n";
      
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Class->getSourceRange()), classContent);

      #if MUT36_OUTPUT
      llvm::errs() << "MUT36: " << "Add usage function: " << instrumentContent << "\n";                 
      #endif
    }
  }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxRecordDecl(
                                  has(cxxMethodDecl(isOverride(), parameterCountIs(0)))
                               ).bind("cxxRecordWithOverrideMethodDecl_0");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *derivedClass  = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("derivedClass")){
    if(!derivedClass ||!Result.Context->getSourceManager().isWrittenInMainFile(derivedClass->getBeginLoc())) return;
    if(!derivedClass->isStruct()&&!derivedClass->isClass()||derivedClass->isLambda()) return;
    if(!derivedClass->isCompleteDefinition()) return;

    if (const auto *TemplateClass = llvm::dyn_cast<ClassTemplateSpecializationDecl>(derivedClass)) {
        return;
    }

    #if MUT37_OUTPUT
    llvm::errs() << "MUT37: " << "Match class with base: " << derivedClass->getNameAsString() << "\n"; 
    #endif

    string instrument4Derived = "";

    for (const auto &base : derivedClass->bases()) {
      const auto *baseType = base.getType().getTypePtr();
      if (const auto *baseRecord = baseType->getAsCXXRecordDecl()) {
        if (const auto *TemplateClass = llvm::dyn_cast<ClassTemplateSpecializationDecl>(baseRecord)) {
            continue;
        }

        #if MUT37_OUTPUT
        llvm::outs() << "\tBase Class: " << baseRecord->getNameAsString() << "\n";
        #endif

        std::string funcRetType = "void";
        std::string baseName = baseRecord->getNameAsString();
        string funcName = "mut37_func_" + baseName;

        if (instrumentedBase.find(baseName) == instrumentedBase.end()) {
          instrumentedBase.insert(baseName);
          std::string instrument4Base = "\t/*mut37*/virtual " + funcRetType + " " + funcName + "_v" + "() = 0;\n";

          Rewrite.InsertText(baseRecord->getEndLoc(), instrument4Base, false, true);

          #if MUT37_OUTPUT
          llvm::errs() << "\t\t" << "Add virtual function: " << instrument4Base << "\n";                 
          #endif
        }

        instrument4Derived += "\t/*mut37*/virtual " + funcRetType + " " + funcName + "_o" + "() override{}\n";

        #if MUT37_OUTPUT
        llvm::errs() << "\t\t" << "Add override function: " << instrument4Derived << "\n";                 
        #endif
      }
    }
    if (instrument4Derived != "") {
      SourceLocation insLoc = derivedClass->getEndLoc();
      Rewrite.InsertText(insLoc, instrument4Derived, false, true);
    }
  }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
  MatchFinder matchFinder;
  DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("derivedClass");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.matchAST(Context);
}