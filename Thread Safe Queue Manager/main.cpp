//
//  main.cpp
//  Thread Safe Queue Manager
//
//  Created by Tim Madorma on 1/3/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//

#include <iostream>
#include <thread>
#include "QueueManager.hpp"

int client_queue_writer(int inc);

using namespace std;

//--------------------------------------------------------------------------
// main()
//--------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
    atomic<bool> execute(true) ;
    
    QueueManager my_queue_mgr(execute);
    
    // Create a vector of read queue threads that will be reused to process
    // "client messages" that are written to the queue.
    vector<thread> rd_queue_thds;
    for (int i = 0; i < 10; i++) {
        rd_queue_thds.push_back(thread(&QueueManager::read_queue, &my_queue_mgr, i));
    }
    
    // Create a vector of simulated "client threads" that will
    /*
     vector<thread> wrt_queue_thds;
     for (int i = 0; i < 10; i++) {
     wrt_queue_thds.push_back(thread(&client_queue_writer, i));
     }
     */
    
    // Simulate multiple "client" write threads writing to the queue.
    for (int j = 0; j < 100; ++j) {
        my_queue_mgr.write_queue("test" + to_string(j));
    }
    
    // Sleep for a second here to see if rd_queue() processes all of the writes
    std::this_thread::sleep_for (std::chrono::seconds(1));
    
    // Change the thread execute flag and notify the cv to stop thier execution
    execute = false;
    my_queue_mgr.notify_cv();
    
    // Wait for each thread in the vector to complete
    for (auto& rd_queue_thd : rd_queue_thds) {
        rd_queue_thd.join();
    }
    return 0;
}

//--------------------------------------------------------------------------
// client_queue_writer
//--------------------------------------------------------------------------
int client_queue_writer(int inc) {
    // NOTE: unique_lock is to be used per thread on a global/shared mutex. Hence
    // the need to instantiate here rather than have it as a member of the class.
    /*
     unique_lock<mutex> queue_lock(mtx);
     
     while (execute_threads) {
     while( msg_queue.empty() && execute_threads) {
     cout << "thread " << thread_num << " checked_but_queue_empty" << endl;
     check_queue_readiness_cv.wait(queue_lock);
     }
     
     if (execute_threads == false) { break; }
     
     cout << "thread " << thread_num << " operating on: " << msg_queue.front() << endl;
     msg_queue.pop();
     
     notify_thrds_to_check_queue();
     
     wait_for_queue_check_signal(queue_lock);
     //check_queue_readiness_cv.wait(queue_lock);
     }
     cout << "thread: " << thread_num << " execute_threads flag is: " << execute_threads << endl;
     */
    return 0;
}

