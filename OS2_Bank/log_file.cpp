//
//  log_file.cpp
//  OS2_Bank
//
//  Created by Elior on 07/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "log_file.hpp"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
using namespace std;

    LogFile::LogFile(){
        pthread_mutex_init(&mutex_log_file, nullptr);
        out.open("log.txt");
    }
    

    LogFile::~LogFile(){
        out.close();
        pthread_mutex_destroy(&mutex_log_file);
    }
    
    void LogFile::lock_log_file() {pthread_mutex_lock(&mutex_log_file);}

    void LogFile::unlock_log_file() {pthread_mutex_unlock(&mutex_log_file);}














// @@@@@@@@@@@@@ LogFile << "Error <ATM ID>: Your transaction failed – account with the same id exists" << endl;

// @@@@@@@@@@@@@ AT THE END  MyFile.close();
