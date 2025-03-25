set(LLVM_LINK_COMPONENTS support)

file(GLOB_RECURSE SOURCES "./[^_]*.h" "./[^_]*.cpp")

# message(STATUS "xxxSources: ${SOURCE_DIR} ${SOURCES}")

add_clang_executable(mutation-instrument
  ${SOURCES}
  main.cpp
  )

target_link_libraries(mutation-instrument
  PRIVATE
  clangFrontend
  clangSerialization
  clangTooling
  clangToolingCore
  clangBasic
  clangAST
  clangASTMatchers
  clangLex
  clangRewrite
  LLVMFrontendOpenMP  
  )

if(CMAKE_VERSION VERSION_GREATER "3.16.0")
target_precompile_headers(mutation-instrument PRIVATE mutator/utils.h)
else()
message(WARNING "CMake version does not support precompiled headers. Consider upgrading CMake.")
endif()
