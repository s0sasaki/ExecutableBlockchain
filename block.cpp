
#include "block.h"
#include "sha256.h"

Block::Block(const int32_t (&data)[DATA_SIZE]){
    nonce = 0;
    timestamp = time(nullptr);
    for(int32_t i=0; i<sizeof(data)/sizeof(int32_t); i++) 
        this->data[i] = data[i];
    hash = calculateHash();
}

void Block::mineBlock(const uint32_t &difficulty){
    char cstr[difficulty + 1];
    for (uint32_t i = 0; i < difficulty; ++i)
        cstr[i] = '0';
    cstr[difficulty] = '\0';
    std::string str(cstr);

    //std::cout << "Mining block... " << std::flush;
    do{
        nonce++;
        hash = calculateHash();
    }while (hash.substr(0, difficulty) != str);
    //std::cout << hash << std::endl;
}

bool Block::isValid(const uint32_t &difficulty) const{
    for(int32_t i=0; i<difficulty; i++)
        if (hash[i] != '0') return false;
    return hash == calculateHash();
}

inline std::string Block::calculateHash() const{
    std::stringstream ss;
    ss << prev_hash << timestamp << nonce;
    for(uint32_t i=0; i<DATA_SIZE; i++)
        ss << data[i];
    return sha256(ss.str());
}
