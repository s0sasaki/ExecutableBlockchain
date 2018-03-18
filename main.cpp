
#include "compiler.h"

int main()
{
    Blockchain blockchain = Blockchain();
    VM vm = VM();
    Compiler compiler = Compiler();
    int32_t bin_data[Block::DATA_SIZE];
    char src_data1[] = "\
    : main              \n\
        1 2 3 4 * + + . \n\
    ; ";
    char src_data2[] = "\
    : square            \n\
        dup *           \n\
    ;                   \n\
    : main              \n\
        5 square .      \n\
    ; ";
    char src_data3[] = "\
    : main              \n\
        10 square .     \n\
    ; ";

    std::cout << src_data1 << std::endl;
    compiler.compile(bin_data, src_data1, blockchain.chain.size());
    vm.exec(bin_data, blockchain);
    blockchain.addBlock(Block(bin_data));
    std::cout << "Block mined: " << blockchain.chain.back().hash << std::endl;

    std::cout << src_data2 << std::endl;
    compiler.compile(bin_data, src_data2, blockchain.chain.size());
    vm.exec(bin_data, blockchain);
    blockchain.addBlock(Block(bin_data));
    std::cout << "Block mined: " << blockchain.chain.back().hash << std::endl;

    std::cout << src_data3 << std::endl;
    compiler.compile(bin_data, src_data3, blockchain.chain.size());
    vm.exec(bin_data, blockchain);
    blockchain.addBlock(Block(bin_data));
    std::cout << "Block mined: " << blockchain.chain.back().hash << std::endl;

    return 0;
}
