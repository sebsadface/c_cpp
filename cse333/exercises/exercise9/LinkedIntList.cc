#include "LinkedIntList.h"

namespace intlist333 {
LinkedIntList::LinkedIntList()
    : num_elements_(0), head_(nullptr), tail_(nullptr) {}

int LinkedIntList::num_elements() const { return this->num_elements_; }

void LinkedIntList::Push(const int payload) {
  Node* new_node = new Node;
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    this->head_ = this->tail_ = new_node;
    new_node->next = nullptr;
  } else {
    new_node->next = this->head_;
    this->head_ = new_node;
  }
  new_node->prev = nullptr;
  this->num_elements_++;
}

void LinkedIntList::Append(const int payload) {
  Node* new_node = new Node;
  new_node->payload = payload;

  if (this->num_elements_ == 0) {
    this->head_ = this->tail_ = new_node;
    new_node->prev = nullptr;
  } else {
    new_node->prev = this->tail_;
    this->tail_ = new_node;
  }
  new_node->next = nullptr;
  this->num_elements_++;
}

bool LinkedIntList::Pop(int* const payload_ptr) {
  if (this->num_elements_ == 0) {
    return false;
  }

  *payload_ptr = this->head_->payload;

  if (this->num_elements_ == 1) {
    this->head_ = this->tail_ = nullptr;
  } else {
    this->head_ = this->head_->next;
    this->head_->prev = nullptr;
  }

  this->num_elements_--;

  return true;
}

bool LinkedIntList::Slice(int* const payload_ptr) {}

}  // namespace intlist333
