//
//  Bank.cpp
//  OS2_Bank
//
//  Created by Elior on 06/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "Bank.hpp"
#include "Account.hpp"
#include <vector>
#include <pthread.h>


class Bank{
    int num_ATM_;
    int balance_;
    int num_readers=0;
    pthread_mutex_t mutex_readers;
    pthread_mutex_t mutex_writers;
    pthread_t print;
    pthread_t commision;
    pthread_t* ATM_list;
    
    
    
    public:
    vector<Account> accountVec;
    
    void lock_readers() {
        pthread_mutex_lock(&mutex_readers);
        num_readers++;
        if (num_readers == 1){
            pthread_mutex_lock(&mutex_writers);
        }
        pthread_mutex_unlock(&mutex_readers);
    }

    void unlock_readers(){
        pthread_mutex_lock(&mutex_readers);
        num_readers--;
        if (num_readers == 0)
            pthread_mutex_unlock(&mutex_writers);
        pthread_mutex_unlock(&mutex_readers);
    }

    void lock_writers() {pthread_mutex_lock(&mutex_writers);}

    void unlock_writers() {pthread_mutex_unlock(&mutex_writers);}

};




