
# Executable Blockchain

An executable blockchain, which consists of:

 - Block Chain with SHA256
 - Virtual Machine with stack-machine instruction sets
 - Compiler for a Forth-like language


## Examples:

**1st Block:**
 Forth uses the so-called Reverse Polish Notation (RPN). 

    ( simple calculation ) 
    : main                 
        1 2 3 4 * + + .    
    ;                      
    
-> 4 * 3 + 2 + 1 = 15

**2nd Block:**
New words are defined and compiled with a : to enter compilation mode and a ; to exit.

    ( simple function definition/call/return )
    : triple                                  
        3 *                                   
    ;                                         
    : main                                    
        5 triple .                            
    ;                                         
    
-> 5 * 3 = 15

**3rd Block:**
"(condition) IF (true-clause) THEN" or 
"(condition) IF (true-clause) ELSE (false-clause) THEN".

    ( simple conditional statement ) 
    : main                       
        2 1 > if 100 then .      
    ;
    
-> 100


**4th Block:**
Nested control structure.

    ( nested conditional statements )
    : main                           
        1 2 + 2 > if                 
            7 2 4 3 - < if           
                0 else               
                7 then               
            7 then                   
        * * .                        
    ; 
-> 7 * 7 * 7

**5th Block:**
Recursive Call.

    ( recursive call )              
    : factorial                     
        dup 1 > if                  
            dup 1 - factorial * then
    ;                               
    : main                          
        5 factorial .               
    ;
-> 5 * 4 * 3 * 2 * 1

**6th Block:**
Tail-call of recursive function.

    ( tail-call )                                   
    : tail_factorial                                
        dup 1 > if                                  
            swap over * swap 1 - tail_factorial then
    ;                                               
    : main                                          
        1 5 tail_factorial drop .                   
    ;
-> 5 * 4 * 3 * 2 * 1


**7th Block:**
Nested function call.

    ( nested function call )
    : sq                    
        dup *               
    ;                       
    : fourth_power          
        sq sq               
    ;                       
    : main                  
        3 fourth_power .    
    ;
-> 3 * 3 * 3 * 3


**8th Block:**
You can use a function from previous blocks.

    ( calling functions in old blocks )
    : main                             
        3 factorial fourth_power .     
    ;
-> ( 3 * 2 * 1 ) * ( 3 * 2 * 1 ) * ( 3 * 2 * 1 ) * ( 3 * 2 * 1 )


**9th Block:**
Volatile variables, which are not recorded in the binary data in the block.

    ( volatile variables ) 
    : main                 
        variable XXX 10    
        variable YYY 20    
        XXX YYY * .        
    ;                      
-> 10 * 20


**10th Block:**
The word ! stores a value at the memory location, and the word @ fetches from the memory. These operations are recorded in the block.

    ( storage variables )
    : main               
        3 XXX !          
        XXX @ .          
    ;                    
-> 3

## References

The hash function is from zedwood.com. I also referred to the sites below:

 - http://www.zedwood.com/article/cpp-sha256-function
 - https://ethereum.github.io/yellowpaper/paper.pdf
 - https://github.com/ethereum/wiki/wiki/Ethereum-Development-Tutorial
 - http://www.complang.tuwien.ac.at/forth/gforth/Docs-html/
 - http://www.forth200x.org/documents/forth16-1.pdf
 - https://github.com/teaandcode/TestChain
 - https://gist.github.com/kumaashi/6d4c83b635fdd996fd67bb8f887bec43

