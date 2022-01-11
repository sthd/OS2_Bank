//
//  ATM.cpp
//  OS2_Bank
//
//  Created by Elior on 03/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "Account.hpp"
#include <iostream>
#include <unistd.h>
#include <cmath>



Account::Account(int ATM, int id, int password, int balance) : id_(id), password_(password), balance_(balance), num_readers(0){
    pthread_mutex_init(&mutex_readers, nullptr);
    pthread_mutex_init(&mutex_writers, nullptr);
    
    logy.lock_log_file();
    logy.out << ATM << ": New account id is " << id_ << " with password " << password_ << " and initial balance " << balance_ << endl;
    logy.unlock_log_file();
}
   
Account::~Account(){

    pthread_mutex_destroy(&mutex_readers);
    pthread_mutex_destroy(&mutex_writers);
}

void Account::cashDeposit(int ATM, int password, int amount){
    lock_writers();
    sleep(1);
    if (password==password_){
        balance_+=amount;
        logy.lock_log_file();
        logy.out << ATM << ": Account " << id_ << " new balance is " << balance_ << " after " << amount << " $ was deposited" << endl;
        logy.unlock_log_file();
    }
    else{
        logy.lock_log_file();
        logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << id_ << " is incorrect" << endl;
        logy.unlock_log_file();
    }
    unlock_writers();
}

void Account::cashWithdrawl(int ATM, int password, int amount){
    lock_writers();
    sleep(1);
    if (password==password_){
        if (balance_ >= amount){
            balance_ -= amount;
            logy.lock_log_file();
            logy.out << ATM << ": Account " << id_ << " new balance is " << balance_ << " after " << amount << " $ was withdrew" << endl;
            logy.unlock_log_file();
        }
        else{
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << id_ << " balance is lower than " << amount << endl;
            logy.unlock_log_file();
        }
    }
    else{
        logy.lock_log_file();
        //write into logfile
        logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << id_ << " is incorrect" << endl;
        logy.unlock_log_file();
    }
    unlock_writers();
}

void Account::checkBalance(int ATM, int password){
   lock_readers();
   sleep(1);
   if (password==password_){
       logy.lock_log_file();
       //write into logfile
       // <ATM ID>: Account <id> balance is <bal>
       logy.out <<ATM << ": Account " << id_ << " balance is " << balance_ << endl;
       logy.unlock_log_file();
   }
   else{
       logy.lock_log_file();
       logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << id_ << " is incorrect" << endl;
       logy.unlock_log_file();
   }
   unlock_readers();
}

int Account::giveCommission(double rate, int percentage){
    lock_writers();
    sleep(1);
    int amount = round(balance_ * rate);
    balance_-=amount;
    logy.lock_log_file();
    logy.out << "Bank: commissions of " << percentage << " % were charged, the bank gained " << amount << " $ from account " << id_ << endl;
    logy.unlock_log_file();
    unlock_writers();
    return amount;
}

    
int Account::getID() const {return id_;}
int Account::getPassword() const {return password_;}
int Account::getBalance() const {return balance_;}
void Account::setBalance(int transferAmount){
    balance_+=transferAmount;
}

void Account::lock_readers() {
    pthread_mutex_lock(&mutex_readers);
    num_readers++;
    if (num_readers == 1){
        pthread_mutex_lock(&mutex_writers);
    }
    pthread_mutex_unlock(&mutex_readers);
}


void Account::unlock_readers(){
    pthread_mutex_lock(&mutex_readers);
    num_readers--;
    if (num_readers == 0)
        pthread_mutex_unlock(&mutex_writers);
    pthread_mutex_unlock(&mutex_readers);
}

void Account::lock_writers() {pthread_mutex_lock(&mutex_writers);}

void Account::unlock_writers() {pthread_mutex_unlock(&mutex_writers);}

