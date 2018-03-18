#include "vm.h"

VM::VM(){
    func_table[OP_RET  ] = &VM::funcOpRet   ;
    func_table[OP_CALL ] = &VM::funcOpCall  ;
    func_table[OP_JMP  ] = &VM::funcOpJmp   ;
    func_table[OP_JZ   ] = &VM::funcOpJz    ;
    func_table[OP_EQ   ] = &VM::funcOpEq    ;
    func_table[OP_GE   ] = &VM::funcOpGe    ;
    func_table[OP_LE   ] = &VM::funcOpLe    ;
    func_table[OP_GNE  ] = &VM::funcOpGne   ;
    func_table[OP_LNE  ] = &VM::funcOpLne   ;
    func_table[OP_PUSH ] = &VM::funcOpPush  ;
    func_table[OP_DROP ] = &VM::funcOpDrop  ;
    func_table[OP_POP  ] = &VM::funcOpPop   ;
    func_table[OP_DUP  ] = &VM::funcOpDup   ;
    func_table[OP_OVER ] = &VM::funcOpOver  ;
    func_table[OP_SWAP ] = &VM::funcOpSwap  ;
    func_table[OP_ADD  ] = &VM::funcOpAdd   ;
    func_table[OP_SUB  ] = &VM::funcOpSub   ;
    func_table[OP_MUL  ] = &VM::funcOpMul   ;
    func_table[OP_DIV  ] = &VM::funcOpDiv   ;
    func_table[OP_DOT  ] = &VM::funcOpDot   ;
    func_table[OP_HALT ] = &VM::funcOpHalt  ;
    func_table[OP_NOP  ] = &VM::funcOpNop   ;
    func_table[OP_LOAD ] = &VM::funcOpLoad  ;
    func_table[OP_STORE] = &VM::funcOpStore ;
}

void VM::exec(const int32_t *bin_data, const Blockchain &blockchain) {
    refresh(bin_data, blockchain);
    while(is_halt == false) {
#ifdef DEBUG
        displayState();
#endif
        (this->*func_table[mem[ip++]])();
    }
}

int32_t VM::peep() const { return data_stack[psp]; }

void VM::refresh(const int32_t *latest_data, const Blockchain &blockchain){
    ib      = blockchain.chain.size();
    ip      = 0;
    rsp     = 0;
    psp     = 0;
    is_halt = false;
    pblockchain = &blockchain;
    std::memcpy(mem,      latest_data, sizeof mem);
    std::memcpy(bin_data, latest_data, sizeof mem);
    std::memset(return_stack, 0, sizeof return_stack);
    std::memset(data_stack, 0, sizeof data_stack);
}

inline void VM::load() {
    if (ib < pblockchain->chain.size())
        std::memcpy(mem, pblockchain->chain[ib].data, sizeof mem);
    else
        std::memcpy(mem, bin_data, sizeof mem);
}

void VM::displayOp(const int32_t &idx) const{
    switch(idx) {
        case OP_RET    : std::cout << "OP_RET  " << std::endl; break;
        case OP_CALL   : std::cout << "OP_CALL " << std::endl; break;
        case OP_JMP    : std::cout << "OP_JMP  " << std::endl; break;
        case OP_JZ     : std::cout << "OP_JZ   " << std::endl; break;
        case OP_EQ     : std::cout << "OP_EQ   " << std::endl; break;
        case OP_GE     : std::cout << "OP_GE   " << std::endl; break;
        case OP_LE     : std::cout << "OP_LE   " << std::endl; break;
        case OP_GNE    : std::cout << "OP_GNE  " << std::endl; break;
        case OP_LNE    : std::cout << "OP_LNE  " << std::endl; break;
        case OP_PUSH   : std::cout << "OP_PUSH " << std::endl; break;
        case OP_POP    : std::cout << "OP_POP  " << std::endl; break;
        case OP_DUP    : std::cout << "OP_DUP  " << std::endl; break;
        case OP_OVER   : std::cout << "OP_OVER " << std::endl; break;
        case OP_SWAP   : std::cout << "OP_SWAP " << std::endl; break;
        case OP_DROP   : std::cout << "OP_DROP " << std::endl; break;
        case OP_ADD    : std::cout << "OP_ADD  " << std::endl; break;
        case OP_SUB    : std::cout << "OP_SUB  " << std::endl; break;
        case OP_MUL    : std::cout << "OP_MUL  " << std::endl; break;
        case OP_DIV    : std::cout << "OP_DIV  " << std::endl; break;
        case OP_NOP    : std::cout << "OP_NOP  " << std::endl; break;
        case OP_DOT    : std::cout << "OP_DOT  " << std::endl; break;
        case OP_HALT   : std::cout << "OP_HALT " << std::endl; break;
        case OP_LOAD   : std::cout << "OP_LOAD " << std::endl; break;
        case OP_STORE  : std::cout << "OP_STORE" << std::endl; break;
        default        : std::cout << "UNKNOWN " << std::endl; break;
    }
}

void VM::displayState() const {
    std::cout << std::endl;
    std::cout << "ib="   << std::setw(4) << std::right << ib      << ", " ;
    std::cout << "ip="   << std::setw(4) << std::right << ip      << ", " ;
    std::cout << "rsp="  << std::setw(4) << std::right << rsp     << ", " ;
    std::cout << "psp="  << std::setw(4) << std::right << psp     << ", " ;
    std::cout << "fnum=" << std::setw(4) << std::right << mem[ip] << ", " ;
    displayOp(mem[ip]);
    for(int32_t i = 0; i <= psp; i++) {
        std::cout << data_stack[i] << " ";
    }
    std::cout << std::endl;
}

void VM::funcOpRet () { 
    if(rsp == 0) is_halt = true; 
    ip = return_stack[rsp--];  
    ib = return_stack[rsp--];  
    load();
}

void VM::funcOpCall() { 
    return_stack[++rsp] = ib; 
    return_stack[++rsp] = ip + 2; 
    ib = mem[ip++];  
    ip = mem[ip];  
    load();
}

void VM::funcOpJmp  () { ip = mem[ip]; }
void VM::funcOpJz   () { if(data_stack[psp] == 0) ip = mem[ip]; else ip++; psp--; }
void VM::funcOpEq   () { data_stack[psp - 1] = data_stack[psp - 1] == data_stack[psp]; psp--; }
void VM::funcOpGe   () { data_stack[psp - 1] = data_stack[psp - 1] <= data_stack[psp]; psp--; }
void VM::funcOpLe   () { data_stack[psp - 1] = data_stack[psp - 1] >= data_stack[psp]; psp--; }
void VM::funcOpGne  () { data_stack[psp - 1] = data_stack[psp - 1] <  data_stack[psp]; psp--; }
void VM::funcOpLne  () { data_stack[psp - 1] = data_stack[psp - 1] >  data_stack[psp]; psp--; }
void VM::funcOpPush () { data_stack[++psp] = mem[ip++];  }
void VM::funcOpDrop () { psp--;  }
void VM::funcOpPop  () { psp--;  }
void VM::funcOpDup  () { data_stack[psp + 1] = data_stack[psp];     psp++;  }
void VM::funcOpOver () { data_stack[psp + 1] = data_stack[psp - 1]; psp++;  }
void VM::funcOpSwap () { int32_t tmp = data_stack[psp]; data_stack[psp] = data_stack[psp-1]; data_stack[psp-1] = tmp; }
void VM::funcOpAdd  () { data_stack[psp - 1] += data_stack[psp];    psp--;  }
void VM::funcOpSub  () { data_stack[psp - 1] -= data_stack[psp];    psp--;  }
void VM::funcOpMul  () { data_stack[psp - 1] *= data_stack[psp];    psp--;  }
void VM::funcOpDiv  () { data_stack[psp - 1] /= data_stack[psp];    psp--;  }
void VM::funcOpDot  () { std::cout << data_stack[psp] << std::endl;  }
void VM::funcOpHalt () { is_halt = true; }
void VM::funcOpNop  () {  }
void VM::funcOpLoad () { data_stack[psp] = storage[data_stack[psp]]; }
void VM::funcOpStore() { storage[data_stack[psp]] = data_stack[psp-1]; psp -= 2; }

