
#include <assert.h>
#include "compiler.h"

int main()
{
    typedef std::pair<char*, int32_t> TestCase;
    std::vector<TestCase> testcases;

    testcases.emplace_back(TestCase((char*) "\
    ( simple calculation )                        \n\
    : main                                        \n\
        1 2 3 4 * + + .                           \n\
    ;                                             \n\
    ", 1+2+3*4));

    testcases.emplace_back(TestCase((char*) "\
    ( simple function definition/call/return )    \n\
    : triple                                      \n\
        3 *                                       \n\
    ;                                             \n\
    : main                                        \n\
        5 triple .                                \n\
    ;                                             \n\
    ", 5*3));

    testcases.emplace_back(TestCase((char*) "\
    ( simple conditional statement )              \n\
    : main                                        \n\
        2 1 > if 100 then .                       \n\
    ;                                             \n\
    ", 100));

    testcases.emplace_back(TestCase((char*) "\
    ( nested conditional statements )             \n\
    : main                                        \n\
        1 2 + 2 > if                              \n\
            7 2 4 3 - < if                        \n\
                0 else                            \n\
                7 then                            \n\
            7 then                                \n\
        * * .                                     \n\
    ;                                             \n\
    ",7*7*7));

    testcases.emplace_back(TestCase((char*) "\
    ( recursive call )                            \n\
    : factorial                                   \n\
        dup 1 > if                                \n\
            dup 1 - factorial * then              \n\
    ;                                             \n\
    : main                                        \n\
        5 factorial .                             \n\
    ;                                             \n\
    ", 5*4*3*2*1));

    testcases.emplace_back(TestCase((char*) "\
    ( tail-call )                                     \n\
    : tail_factorial                                  \n\
        dup 1 > if                                    \n\
            swap over * swap 1 - tail_factorial then  \n\
    ;                                                 \n\
    : main                                            \n\
        1 5 tail_factorial drop .                     \n\
    ;                                                 \n\
    ", 5*4*3*2*1));                                    
                                                       
    testcases.emplace_back(TestCase((char*) "\
    ( nested function call )                      \n\
    : sq                                          \n\
        dup *                                     \n\
    ;                                             \n\
    : fourth_power                                \n\
        sq sq                                     \n\
    ;                                             \n\
    : main                                        \n\
        3 fourth_power .                          \n\
    ;                                             \n\
    ", 3*3*3*3));                                   
                                                    
    testcases.emplace_back(TestCase((char*) "\
    ( calling functions at old blocks )           \n\
    : main                                        \n\
        3 factorial fourth_power .                \n\
    ;                                             \n\
    ", (3*2*1)*(3*2*1)*(3*2*1)*(3*2*1)));

    testcases.emplace_back(TestCase((char*) "\
    ( volatile variables )                        \n\
    : main                                        \n\
        variable XXX 10                           \n\
        variable YYY 20                           \n\
        XXX YYY * .                               \n\
    ;                                             \n\
    ", 10*20));

    testcases.emplace_back(TestCase((char*) "\
    ( storage variables )                         \n\
    : main                                        \n\
        3 XXX !                                   \n\
        XXX @ .                                   \n\
    ;                                             \n\
    ", 3));

    Blockchain blockchain = Blockchain();
    VM vm = VM();
    Compiler compiler = Compiler();
    int32_t bin_data[Block::DATA_SIZE];
    for (auto testcase: testcases){
        std::cout << testcase.first << std::flush;
        compiler.compile(bin_data, testcase.first, blockchain.chain.size());
        vm.exec(bin_data, blockchain);
        assert(vm.peep() == testcase.second);

        std::cout << "Mining block... " << std::flush;
        blockchain.addBlock(Block(bin_data));
        std::cout << blockchain.chain.back().hash << std::endl;
    }

    std::cout << "Checking validity..." << std::endl;
    assert(blockchain.isValid());
    
    std::cout << "Checking unmodifiability..." << std::endl;
    blockchain.chain[1].data[3] = 999;
    assert(!blockchain.isValid());

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
