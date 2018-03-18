
#ifndef TESTCHAIN_BLOCK_H
#define TESTCHAIN_BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>

class Block {
  public:
    constexpr static auto DATA_SIZE = 1024;
    std::string hash;
    std::string prev_hash;
    int32_t data[DATA_SIZE];
    Block(const int32_t (&data)[DATA_SIZE]);
    void mineBlock(const uint32_t &difficulty);
    bool isValid(const uint32_t &difficulty) const;
  private:
    uint32_t nonce;
    time_t timestamp;
    std::string calculateHash() const;
};

#endif //TESTCHAIN_BLOCK_H
