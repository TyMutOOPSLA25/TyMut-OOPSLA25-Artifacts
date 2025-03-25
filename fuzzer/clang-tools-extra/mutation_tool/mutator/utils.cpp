#include<random>
#include "utils.h"
#include <ctime>
#include <sstream>
#include <iomanip>

namespace stringutils{
  string rangetoStr(SourceManager &sm,SourceRange g){
    string text=Lexer::getSourceText(CharSourceRange::getTokenRange(g),sm,LangOptions(),0).str();
    if (text.size() > 0 && (text.at(text.size()-1) == ',')) //the text can be ""
        return Lexer::getSourceText(CharSourceRange::getCharRange(g), sm, LangOptions(), 0).str();
    return text;
  }
  void strReplace(string & target ,string origin,string replaced){
    auto offindex=target.find(origin,0);
    while(offindex!=string::npos){
      auto res=target.replace(offindex,origin.length(),replaced);
      target=res;
      offindex+=origin.length();
      offindex=target.find(origin,offindex);
    }
  }
  string strSlice(string &target,size_t start_pos, size_t end_pos){
    auto res=target.substr(start_pos,end_pos-start_pos+1);
    return res;
  }

  std::string getTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y_%m_%d_%H_%M_%S");
    return oss.str();
  } 

}

namespace getrandom{
    //get a random integer between[0, size]
    int getRandomIndex(int size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0,size);
        return distrib(gen);
    }

    std::vector<std::string> accessSpecifiers = {"public", "protected", "private"};
    string getRandomAccessSepcifier() {
      size_t len = accessSpecifiers.size();
      int x = getRandomIndex(len-1);
      string ret = accessSpecifiers[x];
      return ret;
    }
}
