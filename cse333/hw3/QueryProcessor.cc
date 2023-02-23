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

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

extern "C" {
#include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader*[array_len_];
  itr_array_ = new IndexTableReader*[array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int rank;        // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult> QueryProcessor::ProcessQuery(
    const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;
  vector<list<IdxQueryResult>> idx_results;
  vector<list<DocIDElementHeader>> didlist_vec;
  DocIDTableReader* didtr;
  int i, j;

  for (i = 0; i < array_len_; i++) {
    didtr = itr_array_[i]->LookupWord(query.front());
    if (didtr != nullptr) {
      didlist_vec[i] = didtr->GetDocIDList();
    }
  }

  if (didlist_vec.size() == 0) {
    return final_result;
  }

  for (i = 0; i < array_len_; i++) {
    if (!didlist_vec[i].empty()) {
      for (DocIDElementHeader ele_header : didlist_vec[i]) {
        IdxQueryResult res;
        res.doc_id = ele_header.doc_id;
        res.rank = ele_header.num_positions;
        idx_results[i].push_back(res);
      }
    }
  }

  if (query.size() != 1 && idx_results.size() != 0) {
    for (i = 1; i < query.size(); i++) {
      didlist_vec.clear();
      for (j = 0; j < array_len_; j++) {
        didtr = itr_array_[j]->LookupWord(query[i]);
        if (didtr != nullptr) {
          didlist_vec[j] = didtr->GetDocIDList();
        }
      }

      if (didlist_vec.size() == 0) {
        return final_result;
      }

      for (j = 0; j < array_len_; j++) {
        if (!didlist_vec[j].empty()) {
          for (DocIDElementHeader ele_header : didlist_vec[j]) {
            for (IdxQueryResult res : idx_results[j]) {
              if (res.doc_id == ele_header.doc_id) {
                res.rank += ele_header.num_positions;
              } else {
                idx_results[j].remove(res);
                if (idx_results[j].empty()) {
                  idx_results.erase();
                }
              }
            }
          }
        }
      }
    }
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

}  // namespace hw3
