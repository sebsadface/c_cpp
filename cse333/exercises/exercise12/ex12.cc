/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>

#include "SimpleQueue.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::thread;

static constexpr int kNumSnacks = 6;
static SimpleQueue queue;
static unsigned int seed = time(nullptr);
static pthread_mutex_t write_lock;
const int NUM_THREADS = 3;

// Thread safe print that prints the given str on a line
void thread_safe_print(const string& str) {
  pthread_mutex_lock(&write_lock);
  // Only one thread can hold the lock at a time, making it safe to
  // use cout. If we didn't lock before using cout, the order of things
  // put into the stream could be mixed up.
  cout << str << endl;
  pthread_mutex_unlock(&write_lock);
}

// Produces kNumSnacks snacks of the given type
// You should NOT modify this method at all
void producer(const string& snack_type) {
  for (int i = 0; i < kNumSnacks; i++) {
    queue.Enqueue(snack_type);
    thread_safe_print(snack_type + " ready!");
    int sleep_time = rand_r(&seed) % 500 + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
}

// Eats 2 * kNumSnacks snacks
// You should NOT modify this method at all
void consumer() {
  for (int i = 0; i < kNumSnacks * 2; i++) {
    bool successful = false;
    string snack_type;
    while (!successful) {
      while (queue.IsEmpty()) {
        // Sleep for a bit and then check again
        int sleep_time = rand_r(&seed) % 800 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
      successful = queue.Dequeue(&snack_type);
    }
    thread_safe_print(snack_type + " eaten!");
  }
}

void* thread_producer(void* arg) {
  string* snack = reinterpret_cast<string*>(arg);

  producer(*snack);

  delete snack;
  return nullptr;
}

void* thread_consumer(void*) {
  consumer();
  return nullptr;
}

int main(int argc, char** argv) {
  pthread_mutex_init(&write_lock, nullptr);
  // Your task: Make the two producers and the single consumer
  // all run concurrently (hint: use pthreads)
  pthread_t thds[NUM_THREADS];

  string* arg1 = new string("piroshki");
  string* arg2 = new string("nalysnyky");

  if (pthread_create(&thds[0], nullptr, &thread_producer, arg1) != 0) {
    cerr << "pthread_create failed" << endl;
  }

  if (pthread_create(&thds[1], nullptr, &thread_producer, arg2) != 0) {
    cerr << "pthread_create failed" << endl;
  }

  if (pthread_create(&thds[2], nullptr, &thread_consumer, nullptr) != 0) {
    cerr << "pthread_create failed" << endl;
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(thds[i], nullptr) != 0) {
      cerr << "pthread_join failed" << endl;
    }
  }
  pthread_mutex_destroy(&write_lock);
  return EXIT_SUCCESS;
}
