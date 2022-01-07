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


pthread_mutex_t mutex_log_file{};

void lock_log_file() {pthread_mutex_lock(&mutex_log_file);}

void unlock_log_file() {pthread_mutex_unlock(&mutex_log_file);}



#endif /* log_file_hpp */
