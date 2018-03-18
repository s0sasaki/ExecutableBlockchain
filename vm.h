
#ifndef VM_H
#define VM_H

#include <string.h>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include "blockchain.h"

class VM{
  public:
    enum OpCodes{
        OP_RET, OP_CALL, OP_JMP, OP_JZ, 
        OP_EQ, OP_GE, OP_LE, OP_GNE, OP_LNE,
        OP_PUSH, OP_POP, OP_DUP, OP_OVER, OP_SWAP, OP_DROP,
        OP_ADD, OP_SUB, OP_MUL, OP_DIV, 
        OP_DOT, OP_HALT, OP_NOP, OP_LOAD, OP_STORE,
        OP_MAX
    };
    VM();
    void exec(const int32_t *latest_data, const Blockchain &blockchain);
    int32_t peep() const;
  private:
    constexpr static auto STACK_SIZE   = 1024;
    constexpr static auto STORAGE_SIZE = 1024;
    int32_t ib;
    int32_t ip;
    int32_t rsp;
    int32_t psp;
    bool is_halt;
    const Blockchain *pblockchain; 
    int32_t bin_data[Block::DATA_SIZE];
    int32_t mem[Block::DATA_SIZE];
    int32_t storage[STORAGE_SIZE];
    int32_t return_stack[STACK_SIZE];
    int32_t data_stack[STACK_SIZE];
    void (VM::*func_table[OP_MAX])();
    void refresh(const int32_t *latest_data, const Blockchain &blockchain);
    void load();
    void displayOp(const int32_t &idx) const;
    void displayState() const;
    void funcOpRet  () ;
    void funcOpCall () ;
    void funcOpJmp  () ;
    void funcOpJz   () ;
    void funcOpEq   () ;
    void funcOpGe   () ;
    void funcOpLe   () ;
    void funcOpGne  () ;
    void funcOpLne  () ;
    void funcOpPush () ;
    void funcOpDrop () ;
    void funcOpPop  () ;
    void funcOpDup  () ;
    void funcOpOver () ;
    void funcOpSwap () ;
    void funcOpAdd  () ;
    void funcOpSub  () ;
    void funcOpMul  () ;
    void funcOpDiv  () ;
    void funcOpDot  () ;
    void funcOpHalt () ;
    void funcOpNop  () ;
    void funcOpLoad () ;
    void funcOpStore() ;
};

#endif /* VM_H */

