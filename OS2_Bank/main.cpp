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

// declare threads

class LogFile logy;


int main(int argc, const char * argv[]) {

    logy.lock_log_file();
    // create threads + run them
    Bank Lloyds(15);
    Lloyds.openAccount(12, 1234, 1111, 20);
    	
    
    
    //join threads
    
    //free memory + last prints
    
    return 0;
}
