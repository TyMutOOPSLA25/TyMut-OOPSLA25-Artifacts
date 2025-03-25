# TyMut-OOPSLA25-Artifacts
This is the project repository of our OOPSLA 2025 research paper: Fuzzing C++ Compilers via Type-Driven Mutation. The project records our mutation-based C++ fuzzer, **TyMut** and the corresponding bugs uncovered by it during the complete running phase. The detailed project layout is shown below.

### Project Layout Overview
***
Here are the main useful files and directories:
```
|-- TyMutOOPSLA25Artifact/
    |-- fuzzer/    ## The complete fuzzer project with LLVM toolchain
        |-- clang-tools-extra/mutation_tool/    ## Source code of mutators

    |-- uncovered_bugs/    
        |-- uncovered_bugs.xlsx    ## Uncovered bug list recorded by TyMut
```

### Run the Fuzzer
***
Installing **TyMut** from source code is supported by using the following commands:

```bash
cd ./fuzzer/
mkdir build && cd build
cmake -G Ninja ../llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_BUILD_TESTS=ON  -DCMAKE_BUILD_TYPE="Release" -DLLVM_ENABLE_ASSERTIONS=ON -DBUILD_SHARED_LIBS=ON
ninja
```

To run a single mutator, use the following command:

```bash
./fuzzer/build/bin/mutation-instrument <seed-file-name> --mutator=<mutator-id> --sourcedir=<seed-file-directory> --
```

### Uncovered Bugs
***
Table `/uncovered_bugs/uncovered_bugs.xlsx` records all the bugs reported by **TyMut** during its running phase, with the corresponding **ISSUE IDs**, status, and source code(in the form of [godbolt link](https://godbolt.org/)). Note that we intentionally anonymize all the **ISSUE IDs** of the bugs in both the paper and this table for double-blind policy(like *LLVM-9XX66*), as we used real-name submission when reporting these issues.




 
