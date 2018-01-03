//
//  QueueManager.hpp
//  Thread Safe Queue Manager
//
//  Created by Tim Madorma on 1/3/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//

#ifndef QueueManager_hpp
#define QueueManager_hpp

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>

class QueueManager
{
public:
    QueueManager(std::atomic<bool> &run): execute_threads(run){}
    
    int write_queue(std::string write_message);
    int read_queue(int);
    void notify_cv(void) { check_queue_readiness_cv.notify_all(); }
    
private:
    inline void notify_thrds_to_check_queue(void) { check_queue_readiness_cv.notify_all(); }
    inline void wait_for_queue_check_signal(std::unique_lock<std::mutex> &unique_lock_ref)
    { check_queue_readiness_cv.wait(unique_lock_ref); }
    
    std::mutex mtx;                                     // shared mutex managed by per thread queue_lock
    std::atomic<bool> &execute_threads;                 // flag telling if we should execute or not
    std::condition_variable check_queue_readiness_cv;   // condition_variable for accessing msg_queue
    std::queue <std::string> msg_queue;                 // The msg_queue that the threads will write to and read
};

#endif /* QueueManager_hpp */
