// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#ifndef LINKEDINTLIST_H_
#define LINKEDINTLIST_H_

#include <memory>
#include "IntList.h"

using std::shared_ptr;
using std::weak_ptr;

namespace intlist333 {

// LinkedIntList is an implementation of
// the IntList abstract class. Implemented using
// a doubly linked list
class LinkedIntList : public IntList {
 public:
  // Default: construct a LinkedIntList with 0 num_elements, empty head and tail
  LinkedIntList();

  // Disable copy constructor
  LinkedIntList(const LinkedIntList& other) = delete;

  // Destructor should be kept default
  // since smart pointers should clean
  // everything up for us
  virtual ~LinkedIntList() = default;

  // Disable assignment operator
  LinkedIntList& operator=(const LinkedIntList& rhs) = delete;

  int num_elements() const override;
  void Push(const int payload) override;
  void Append(const int payload) override;
  bool Pop(int* const payload_ptr) override;
  bool Slice(int* const payload_ptr) override;

 private:
  // Number of elements in the LinkedIntList
  int num_elements_;

  // A single node within a linked list.
  //
  // A node contains next and prev pointers as well as a customer-supplied
  // integer payload.
  struct Node {
    int payload;            // ustomer-supplied integer payload
    shared_ptr<Node> next;  // next node in list, or empty
    weak_ptr<Node> prev;    // prev node in the list, or empty
  };

  shared_ptr<Node> head_;  // head of linked list, or empty
  shared_ptr<Node> tail_;  // tail of linked list, or empty
};

}  // namespace intlist333

#endif  // LINKEDINTLIST_H_
