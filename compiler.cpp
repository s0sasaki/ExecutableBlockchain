
#include "compiler.h"

inline bool Compiler::isSep(char c, const char *s) const{
    while(*s) 
        if(c == *s++) return true;
    return false;
}

inline bool Compiler::isNum(const char *s) const{
    while(*s) 
        if(!isdigit(*s++)) return false;
    return true;
}

void Compiler::read(std::vector<std::string> &strtable, const char *src_data) const{
    int32_t c;
    int32_t idx = 0;
    char buf[256];
    bool iscomment = false;
    while((c = *src_data++)) {
        c = toupper(c);
        if(isSep(c, (char *)" \t\r\n()")) {
            if(c == '(') iscomment = true;
            if(c == ')') iscomment = false;
            if(!idx) continue;
            buf[idx] = 0;
            strtable.push_back(buf);
            idx = 0;
            continue;
        }
        if(!iscomment) buf[idx++] = c;
    }
}

void Compiler::compile(int32_t *bin_data, const char *src_data, uint32_t bc_length) {
    std::vector<std::string> strtable;
    int32_t rsp = 0;
    int32_t rspbuf[256];
    int32_t i    = 0;
    int32_t midx = 2; 
    bool capture = false;
    std::string recur = "";
    read(strtable, src_data);
    while(i < strtable.size()) {
        std::string wordstr = strtable[i++];

        if(var_dict.count(wordstr)>0){
            bin_data[midx++] = VM::OP_PUSH;
            bin_data[midx++] = var_dict[wordstr];
            continue;
        }
        
        if(wordstr == "VARIABLE"){
            var_dict[strtable[i]] = (int32_t) stoi(strtable[i+1]);
            i+=2;
            continue;
        }

        if(capture) {
            func_dict[wordstr] = std::make_pair(midx, bc_length);
            capture = false;
            recur = wordstr;
            continue;
        }

        if(wordstr == ":")    {
            capture = true;
            continue;
        }

        if(wordstr == ";")    {
            bin_data[midx++] = VM::OP_RET;
            recur       = "";
            continue;
        }

        if(func_dict.count(wordstr) > 0){
            if(recur == wordstr && strtable[i] == "THEN" && strtable[i+1] == ";") {
                bin_data[midx++] = VM::OP_JMP;
                bin_data[midx++] = func_dict[wordstr].first;
                continue;
            }
            bin_data[midx++] = VM::OP_CALL;
            bin_data[midx++] = func_dict[wordstr].second;
            bin_data[midx++] = func_dict[wordstr].first;
            continue;
        }

        if(isNum(wordstr.c_str())) {
            bin_data[midx++] = VM::OP_PUSH;
            int32_t val = stoi(wordstr);
            bin_data[midx++] = val;
            continue;
        }

        if(wordstr == "IF") {
            bin_data[midx++] = VM::OP_JZ;
            rspbuf[++rsp] = midx++;
            continue;
        }

        if(wordstr == "ELSE") {
            int32_t rsptemp = rspbuf[rsp--];
            bin_data[midx++] = VM::OP_JMP;
            rspbuf[++rsp] = midx++;
            bin_data[rsptemp] = midx;
            continue;
        }

        if(wordstr == "THEN") {
            int32_t rsptemp = rspbuf[rsp--];
            bin_data[rsptemp] = midx;
            continue;
        }

        if(wordstr == "+")    { bin_data[midx++] = VM::OP_ADD;   continue; }
        if(wordstr == "-")    { bin_data[midx++] = VM::OP_SUB;   continue; }
        if(wordstr == "*")    { bin_data[midx++] = VM::OP_MUL;   continue; }
        if(wordstr == "/")    { bin_data[midx++] = VM::OP_DIV;   continue; }
        if(wordstr == ".")    { bin_data[midx++] = VM::OP_DOT;   continue; }
        if(wordstr == "<=")   { bin_data[midx++] = VM::OP_GE;    continue; }
        if(wordstr == ">=")   { bin_data[midx++] = VM::OP_LE;    continue; }
        if(wordstr == "<")    { bin_data[midx++] = VM::OP_GNE;   continue; }
        if(wordstr == ">")    { bin_data[midx++] = VM::OP_LNE;   continue; }
        if(wordstr == "HALT") { bin_data[midx++] = VM::OP_HALT;  continue; }
        if(wordstr == "POP")  { bin_data[midx++] = VM::OP_POP;   continue; }
        if(wordstr == "DROP") { bin_data[midx++] = VM::OP_DROP;  continue; }
        if(wordstr == "DUP")  { bin_data[midx++] = VM::OP_DUP;   continue; }
        if(wordstr == "OVER") { bin_data[midx++] = VM::OP_OVER;  continue; }
        if(wordstr == "SWAP") { bin_data[midx++] = VM::OP_SWAP;  continue; }
        if(wordstr == "@")    { bin_data[midx++] = VM::OP_LOAD;  continue; }
        if(wordstr == "!")    { bin_data[midx++] = VM::OP_STORE; continue; }

        std::cout << "ERROR UNKNOWN WORD: " << wordstr << std::endl;
        for(int32_t i = 0; i < Block::DATA_SIZE; i++) {
            bin_data[i] = VM::OP_HALT;
        }
        return;
    }
    if (func_dict.count("MAIN") > 0 && func_dict["MAIN"].second == bc_length ){
        bin_data[0]  = VM::OP_JMP;
        bin_data[1]  = func_dict["MAIN"].first;
    } else {
        bin_data[0]  = VM::OP_HALT;
        bin_data[1]  = VM::OP_HALT;
    }
}

