//
//  QueueManager.cpp
//  Thread Safe Queue Manager
//
//  Created by Tim Madorma on 1/3/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//

#include "QueueManager.hpp"

using namespace std;

//--------------------------------------------------------------------------
// QueueManager::read_queue - This is the thread function to read the queue.
//--------------------------------------------------------------------------
int QueueManager::read_queue(int thread_num) {
    
    // NOTE: unique_lock is to be used per thread on a global/shared mutex. Hence
    // the need to instantiate here rather than have it as a member of the class.
    unique_lock<mutex> queue_lock(mtx);
    
    while (execute_threads) {
        while( msg_queue.empty() && execute_threads) {
            cout << "thread " << thread_num << " checked queue but it is empty" << endl;
            check_queue_readiness_cv.wait(queue_lock);
        }
        
        if (execute_threads == false) { break; }
        
        cout << "thread " << thread_num << " operating on message: " << msg_queue.front() << endl;
        msg_queue.pop();
        
        notify_thrds_to_check_queue();
        wait_for_queue_check_signal(queue_lock);
    }
    cout << "thread " << thread_num << " execute_threads flag is: " << execute_threads << endl;
    return 0;
}

//----------------------------------------------------------------------
// QueueManager::write_queue
//
// A thread safe callback function utilized by "client" thread writers
//----------------------------------------------------------------------
int QueueManager::write_queue(string write_message) {
    msg_queue.push(write_message);
    notify_cv();
    return 0;
}
