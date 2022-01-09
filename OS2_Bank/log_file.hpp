//
//  log_file.hpp
//  OS2_Bank
//
//  Created by Elior on 07/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#ifndef log_file_hpp
#define log_file_hpp

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

using namespace std;


class LogFile{
        pthread_mutex_t mutex_log_file{};

    
    public:
        ofstream out;
    
        LogFile();
        
        ~LogFile();
        
        void lock_log_file();
        
        void unlock_log_file();
    
};

// @@@@@@@@@@@@@ logFile << "Error <ATM ID>: Your transaction failed – account with the same id exists" << endl;

// @@@@@@@@@@@@@ AT THE END  MyFile.close();

#endif /* log_file_hpp */
