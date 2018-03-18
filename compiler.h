
#ifndef COMPILER_H
#define COMPILER_H

#include "vm.h"

class Compiler{
  public:
    void compile(int32_t *bin_data, const char *src_data, uint32_t bc_length);
  private:
    std::map<std::string, int32_t> var_dict;
    std::map<std::string, std::pair<int32_t, int32_t> > func_dict;
    bool isSep(char c, const char *s) const ;
    bool isNum(const char *s) const;
    void read(std::vector<std::string> &strtable, const char *src_data) const;
};

#endif /* COMPILER_H */

