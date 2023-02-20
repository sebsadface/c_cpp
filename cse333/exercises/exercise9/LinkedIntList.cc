// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include "LinkedIntList.h"

namespace intlist333 {

// Default constructure
LinkedIntList::LinkedIntList()
    : num_elements_(0), head_(new Node), tail_(new Node) {}

// Get the number of element in the list
int LinkedIntList::num_elements() const { return this->num_elements_; }

// Push a new node to the front of the list
void LinkedIntList::Push(const int payload) {
  shared_ptr<Node> new_node(new Node);
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    // When the list is empty, set both the head and the tail to the new node
    this->head_ = this->tail_ = new_node;
  } else {
    // Otherwise, link the new node to the current head and set the head to
    // the new node
    this->head_->prev = new_node;
    new_node->next = this->head_;
    this->head_ = new_node;
  }

  // Update element counter
  this->num_elements_++;
}

// Append a new node to the end of the list
void LinkedIntList::Append(const int payload) {
  shared_ptr<Node> new_node(new Node);
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    // When the list is empty, set both the head and the tail to the new node
    this->head_ = this->tail_ = new_node;
  } else {
    // Otherwise, link the new node to the current tail and set the tail to
    // the new node
    this->tail_->next = new_node;
    new_node->prev = this->tail_;
    this->tail_ = new_node;
  }

  // Update the element counter
  this->num_elements_++;
}

// Removes the first element in the list
bool LinkedIntList::Pop(int* const payload_ptr) {
  // If the list is empty or the output pointer is null, return false
  if (this->num_elements_ == 0 || payload_ptr == nullptr) {
    return false;
  }

  *payload_ptr = this->head_->payload;

  if (this->num_elements_ == 1) {
    // If there is only one element in the list, we set the head and the tail to
    // empty
    this->head_.reset();
    this->tail_.reset();
  } else {
    // Update the head
    this->head_ = this->head_->next;
    this->head_->prev.reset();
  }

  // Update the element counter
  this->num_elements_--;

  return true;
}

// Removes the last element in the list
bool LinkedIntList::Slice(int* const payload_ptr) {
  // If the list is empty or the output pointer is null, return false
  if (this->num_elements_ == 0 || payload_ptr == nullptr) {
    return false;
  }

  *payload_ptr = this->tail_->payload;

  if (this->num_elements_ == 1) {
    // If there is only one element in the list, we set the head and the tail to
    // empty
    this->head_.reset();
    this->tail_.reset();
  } else {
    // Update the tail
    this->tail_ = this->tail_->prev.lock();
    this->tail_->next.reset();
  }

  // Update the element counter
  this->num_elements_--;

  return true;
}

}  // namespace intlist333
