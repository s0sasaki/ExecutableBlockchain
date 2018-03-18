
#ifndef TESTCHAIN_BLOCKCHAIN_H
#define TESTCHAIN_BLOCKCHAIN_H

#include <cstdint>
#include <vector>
#include "block.h"

class Blockchain {
  public:
    std::vector<Block> chain;
    Blockchain();
    void addBlock(Block block);
    bool isValid() const;
  private:
    const uint32_t difficulty = 3;
};

#endif //TESTCHAIN_BLOCKCHAIN_H
