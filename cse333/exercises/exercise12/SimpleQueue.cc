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

#include "SimpleQueue.h"

#include <memory>

using std::string;
using std::shared_ptr;

SimpleQueue::SimpleQueue() {
  this->size_ = 0;
  this->front_.reset();
  this->end_.reset();
}

SimpleQueue::~SimpleQueue() {

}

void SimpleQueue::Enqueue(const string& item) {
  shared_ptr<Node> new_node(new Node());
  new_node->next.reset();
  new_node->item = item;
  if (this->end_) {
    this->end_->next = new_node;
  } else  {
    this->front_ = new_node;
  }
  this->end_ = new_node;
  this->size_++;
}

bool SimpleQueue::Dequeue(string* const result) {
  if (this->size_ == 0) {
    return false;
  }
  *result = this->front_->item;
  if (this->end_ == this->front_) {
    this->end_ = this->front_ = this->front_->next;
  } else {
    this->front_ = this->front_->next;
  }
  this->size_--;
  return true;
}

int SimpleQueue::Size() const {
  return this->size_;
}

bool SimpleQueue::IsEmpty() const {
  return this->size_ == 0;
}
