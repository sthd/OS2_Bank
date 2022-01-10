//
//  main.cpp
//  OS2_Bank
//
//  Created by Elior on 03/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include <iostream>
#include "Bank.cpp"
#include "log_file.hpp"
#include <cmath>
// declare threads

class LogFile logy;

int main(int argc, const char * argv[]) {

    logy.lock_log_file();
    // create threads + run them
    
    
    
    Bank Lloyds(15);
    Lloyds.openAccount(12, 1234, 1111, 23);
    Lloyds.openAccount(12, 1234, 1111, 0);
    
    //Lloyds.closeAccount(13, 1234, 1110);
    //Lloyds.closeAccount(14, 1234, 1111);
    Lloyds.moneyTransfer(12, 1321, 2222, 3214, 30);  // no source
    Lloyds.moneyTransfer(12, 1234, 2222, 3214, 30); // no password
    Lloyds.moneyTransfer(12, 1234, 1111, 3214, 30); // no money
    Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // no target
    Lloyds.openAccount(12, 3214, 1111, 25);
    
    Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // should work

    //join threads
    
    
    //free memory + last prints
    
    return 0;
}
