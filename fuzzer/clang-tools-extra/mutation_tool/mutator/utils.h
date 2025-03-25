#pragma once
#include "toolings.h"
#include<random>
#include <unordered_set>
#include <unordered_map>
namespace stringutils{
  string rangetoStr(SourceManager &sm,SourceRange g);
  void strReplace(string & target ,string origin,string replaced);
  string strSlice(string &target,size_t start_pos, size_t end_pos);
  string getTimestamp();

}

namespace getrandom{
    //get a random integer between[0, size]
    int getRandomIndex(int size);
    string getRandomAccessSepcifier();
}
