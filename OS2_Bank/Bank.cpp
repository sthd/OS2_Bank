//
//  Bank.cpp
//  OS2_Bank
//
//  Created by Elior on 06/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "Bank.hpp"
#include "Account.hpp"
#include "log_file.hpp"

#include <vector>
#include <pthread.h>
#include<algorithm>

extern class LogFile logy;

using std::vector;

class Bank{
    int numATM_;
    int balance_;
    int numReaders_;
    pthread_mutex_t mutex_readers;
    pthread_mutex_t mutex_writers;
    
    public:
        
    vector<Account*> accountVector;
    
    Bank(int numATM) : numATM_(numATM), balance_(0), numReaders_(0){
        pthread_mutex_init(&mutex_readers, nullptr);
        pthread_mutex_init(&mutex_writers, nullptr);
        // ATM_list = new pthread_t[numATM_];
        }

    ~Bank(){
        //delete [] ATM_list;
        pthread_mutex_destroy(&mutex_readers);
        pthread_mutex_destroy(&mutex_writers);
    }
    
    void openAccount(int ATM, int accountId, int password, int amount){
        lock_writers();
        sleep(1);
        cout << "BANK: OPEN ACCOUNT: with account id:" << accountId << endl;
        if (findAccount(accountId) == NULL){
            Account* tmp = new Account(ATM, accountId, password, amount);
            accountVector.push_back(tmp);
            //delete tmp;
            //cout << "I'm here: ATM: " << ATM << " accountId " << tmp->getID() << " password " << tmp->getPassword() << " amount " << tmp->getBalance() << endl;
            //accountVector.emplace_back(ATM, accountId, password, amount);
            
        }
        else{
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account with the same id exists" << accountId << endl;
            logy.unlock_log_file();
        }
        unlock_writers();
    }
    
    // ###################### when do we minimise ?????
    void closeAccount(int ATM, int accountId, int password){
        lock_writers();
        sleep(1);
        for (std::vector<Account*>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            if ((*it)->getID() == accountId){
                if ((*it)->getPassword() == password){
                    logy.lock_log_file();
                    logy.out << ATM << ": Account " << accountId << " is now closed. Balance was " << (*it)->getBalance() << endl;
                    accountVector.erase(it);
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
                else{
                    logy.lock_log_file();
                    logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
            }
        }
        logy.lock_log_file();
        logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
        logy.unlock_log_file();
        unlock_writers();
    }
    
    void cashDeposit(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
            logy.unlock_log_file();
        }
        else{
            tmp->cashDeposit(ATM, password, amount);
        }
        unlock_readers();
    }
    
    void cashBalance(int ATM, int accountId, int password){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
            logy.unlock_log_file();
        }
        else{
            tmp->checkBalance(ATM, password);
        }
        unlock_readers();
    }
    
    void cashWithdrawl(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
            logy.unlock_log_file();
        }
        else{
            tmp->cashWithdrawl(ATM, password, amount);
        }
        unlock_readers();
    }
    
    void takeCommission(){
        lock_readers();
        
        double rates[3]={0.02, 0.03, 0.04};
        double rate= rates[int(rand() % 3)];
        int percentage = rate*100;
        for (std::vector<Account*>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            balance_+= (*it)->giveCommission(rate, percentage);
        }
        unlock_readers();
    }

    
    void accountsStatus(){
        
        lock_readers();
        /*
        sort(accountVector.begin(),accountVector.end(), [](const Account& lhs, const Account& rhs) {
            return lhs.getID() < rhs.getID();
        });
         */
        /*
            printf("\033[2J");
            printf("\033[1;1H");
            cout << "Current Bank Status" << endl;
            for (std::vector<Account>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
                cout << "Account " << it->getID() << ": Balance - " << it->getBalance() << " $ , Account Password - " << it->getPassword() << endl;
            }
            cout << "The Bank has " << balance_ << endl;
            */

        unlock_readers();
    }
    
    void moneyTransfer(int ATM, int sourceAccountId, int password, int targetAccountId, int amount){
        lock_readers();
        int minId;
        int maxId;
        bool sourceIsMin=false;
        bool targetExist=true;
        Account* maxAccount;
        Account* sourceAccount;
        Account* targetAccount;
        
        if (sourceAccountId < targetAccountId){
            minId = sourceAccountId;
            maxId = targetAccountId;
            sourceIsMin = true;
        }
        else {
             minId = targetAccountId;
             maxId = sourceAccountId;
         }
        Account* minAccount = findAccount(minId);
        if (minAccount == NULL){
            maxAccount = findAccount(maxId);
            if (maxAccount == NULL){
                logy.lock_log_file();
                logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                logy.unlock_log_file();
                unlock_readers();
                return;
            }
            maxAccount->lock_writers();
            if (sourceIsMin){
                logy.lock_log_file();
                logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                logy.unlock_log_file();
                maxAccount->unlock_writers();
                unlock_readers();
                return;
            }
            targetExist=false;
        }
        else{
            minAccount->lock_writers();
            maxAccount = findAccount(maxId);
            if (maxAccount == NULL){
                if (sourceIsMin == false){
                    logy.lock_log_file();
                    logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                    logy.unlock_log_file();
                    minAccount->unlock_writers();
                    unlock_readers();
                    return;
                }
                targetExist=false;
            }
            else{
                maxAccount->lock_writers();
            }
        }
        if (sourceIsMin){
            sourceAccount = minAccount;
            targetAccount = maxAccount;
        }
        else{
            sourceAccount = maxAccount;
            targetAccount = minAccount;
        }
        
        if (sourceAccount->getPassword() != password){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << sourceAccountId << " is incorrect" << endl;
            logy.unlock_log_file();
            if (targetExist){
                targetAccount->unlock_writers();
            }
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        if (sourceAccount->getBalance() < amount){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " balance is lower than " << amount << endl;
            logy.unlock_log_file();
            if (targetExist){
                targetAccount->unlock_writers();
            }
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        if (targetExist == false){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << targetAccountId << " does not exist" << endl;
            logy.unlock_log_file();
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        sourceAccount->setBalance(-amount);
        targetAccount->setBalance(amount);
        logy.lock_log_file();
        logy.out << ATM << ": Transfer " << amount << " from account " << sourceAccountId << " to account " << targetAccountId << " new account balance is " << sourceAccount->getBalance() << " new target account balance is " << targetAccount->getBalance() << endl;
        logy.unlock_log_file();
        minAccount->unlock_writers();
        maxAccount->unlock_writers();
        unlock_readers();
        return;
    }

    void lock_readers() {
        pthread_mutex_lock(&mutex_readers);
         numReaders_++;
        if (numReaders_ == 1){
            pthread_mutex_lock(&mutex_writers);
        }
        pthread_mutex_unlock(&mutex_readers);
    }

    void unlock_readers(){
        pthread_mutex_lock(&mutex_readers);
        numReaders_--;
        if ( numReaders_ == 0)
            pthread_mutex_unlock(&mutex_writers);
        pthread_mutex_unlock(&mutex_readers);
    }

    void lock_writers() {pthread_mutex_lock(&mutex_writers);}

    void unlock_writers() {pthread_mutex_unlock(&mutex_writers);}

    Account* findAccount(int accountId){
        for (std::vector<Account*>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            if ( (*it)->getID() == accountId){
                return *it;
            }
        }
        return NULL;
    }
    
    void freeAccounts(){
        cout << "Freeing accounts:"<< endl;
        for (std::vector<Account*>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            cout << "Element is: " << (*it)->getID() ;
            if ((*it) != NULL)
                delete (*it);
            cout << " end free" << endl;
            //accountVector.erase(it);
        }
    }
    
};
