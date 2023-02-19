#ifndef LINKEDINTLIST_H_
#define LINKEDINTLIST_H_

#include <memory>
#include "IntList.h"

namespace intlist333 {

// LinkedIntList is an implementation of
// the IntList abstract class. Implemented using
// a doubly linked list
class LinkedIntList : public IntList {
 public:
  LinkedIntList();
  LinkedIntList(const LinkedIntList& other) = delete;
  // Destructor should be kept default
  // since smart pointers should clean
  // everything up for us
  virtual ~LinkedIntList() = default;
  LinkedIntList& operator=(const LinkedIntList& rhs) = delete;

  int num_elements() const override;
  void Push(const int payload) override;
  void Append(const int payload) override;
  bool Pop(int* const payload_ptr) override;
  bool Slice(int* const payload_ptr) override;

 private:
  int num_elements_;
  // Note that these should be modified
  // to use smart pointers at some point
  struct Node {
    int payload;
    Node* next;
    Node* prev;
  };
  Node* head_;
  Node* tail_;
};

}  // namespace intlist333

#endif  // LINKEDINTLIST_H_
