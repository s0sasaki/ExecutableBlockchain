
#include "blockchain.h"

Blockchain::Blockchain(){
    int32_t genesis_data[Block::DATA_SIZE];
    Block genesis_block(genesis_data);
    genesis_block.mineBlock(difficulty);
    chain.emplace_back(genesis_block);
}

bool Blockchain::isValid() const{
    for (auto block: chain)
        if(!block.isValid(difficulty)) return false;
    std::string prev_hash = chain.front().hash;
    for(uint32_t i=1; i<chain.size(); i++){
        if(prev_hash != chain[i].prev_hash) return false;
        prev_hash = chain[i].hash;
    }
    return true;
}

void Blockchain::addBlock(Block block){
    block.prev_hash = chain.back().hash;
    block.mineBlock(difficulty);
    chain.push_back(block);
}

