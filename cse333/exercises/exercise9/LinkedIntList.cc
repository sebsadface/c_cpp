#include "LinkedIntList.h"

namespace intlist333 {
LinkedIntList::LinkedIntList()
    : num_elements_(0), head_(new Node), tail_(new Node) {}

int LinkedIntList::num_elements() const { return this->num_elements_; }

void LinkedIntList::Push(const int payload) {
  shared_ptr<Node> new_node(new Node);
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    this->head_ = this->tail_ = new_node;
  } else {
    this->head_->prev = new_node;
    new_node->next = this->head_;
    this->head_ = new_node;
  }
  this->num_elements_++;
}

void LinkedIntList::Append(const int payload) {
  shared_ptr<Node> new_node(new Node);
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    this->head_ = this->tail_ = new_node;
  } else {
    this->tail_->next = new_node;
    new_node->prev = this->tail_;
    this->tail_ = new_node;
  }
  this->num_elements_++;
}

bool LinkedIntList::Pop(int* const payload_ptr) {
  if (this->num_elements_ == 0) {
    return false;
  }

  *payload_ptr = this->head_->payload;

  if (this->num_elements_ == 1) {
    this->head_.reset();
    this->tail_.reset();
  } else {
    this->head_ = this->head_->next;
    this->head_->prev.reset();
  }

  this->num_elements_--;

  return true;
}

bool LinkedIntList::Slice(int* const payload_ptr) {
  if (this->num_elements_ == 0) {
    return false;
  }

  *payload_ptr = this->tail_->payload;

  if (this->num_elements_ == 1) {
    this->head_.reset();
    this->tail_.reset();
  } else {
    this->tail_ = this->tail_->prev.lock();
    this->tail_->next.reset();
  }

  this->num_elements_--;

  return true;
}

}  // namespace intlist333
