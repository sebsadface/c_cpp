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

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int rank;        // The rank of the result so far.
} IdxQueryResult;

static bool ProcessAdditionalWords(IndexTableReader** const itr_array,
                                   list<IdxQueryResult>& reslist,
                                   const vector<string>& query, int idxfilenum);

static bool FindFileName(const vector<QueryProcessor::QueryResult> final_result,
                         const string& filename);

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

vector<QueryProcessor::QueryResult> QueryProcessor::ProcessQuery(
    const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;
  list<DocIDElementHeader> idlist;
  DocIDTableReader* didtr;
  list<IdxQueryResult> reslist;
  IdxQueryResult res;
  int i;

  for (i = 0; i < array_len_; i++) {
    didtr = itr_array_[i]->LookupWord(query[0]);
    if (didtr == nullptr) {
      continue;
    }

    idlist = didtr->GetDocIDList();
    for (DocIDElementHeader docid_header : idlist) {
      res.doc_id = docid_header.doc_id;
      res.rank = docid_header.num_positions;
      reslist.push_back(res);
    }
    delete (didtr);

    if (query.size() > 1) {
      if (!ProcessAdditionalWords(itr_array_, reslist, query, i)) {
        continue;
      }
    }

    for (IdxQueryResult res : reslist) {
      string filename;
      Verify333(dtr_array_[i]->LookupDocID(res.doc_id, &filename));

      if (!FindFileName(final_result, filename)) {
        QueryProcessor::QueryResult qres;
        qres.document_name = filename;
        qres.rank = res.rank;
        final_result.push_back(qres);
      }
    }
    reslist.clear();
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

static bool ProcessAdditionalWords(IndexTableReader** const itr_array,
                                   list<IdxQueryResult>& reslist,
                                   const vector<string>& query,
                                   int idxfilenum) {
  list<DocIDElementHeader> idlist;
  list<DocPositionOffset_t> poslist;
  DocIDTableReader* didtr;
  uint32_t i, j, num_docs;
  for (i = 1; i < query.size(); i++) {
    didtr = itr_array[idxfilenum]->LookupWord(query[i]);
    if (didtr == nullptr) {
      reslist.clear();
      return false;
    }

    num_docs = reslist.size();
    list<IdxQueryResult>::iterator reslist_itr = reslist.begin();
    for (j = 0; j < num_docs; j++) {
      if (didtr->LookupDocID(reslist_itr->doc_id, &poslist)) {
        reslist_itr->rank += poslist.size();
        reslist_itr++;
      } else {
        reslist_itr = reslist.erase(reslist_itr);
      }
    }
  }

  delete (didtr);
  if (reslist.size() == 0) {
    return false;
  }

  return true;
}

static bool FindFileName(const vector<QueryProcessor::QueryResult> final_result,
                         const string& filename) {
  uint32_t i;
  for (i = 0; i < final_result.size(); i++) {
    if (final_result[i].document_name == filename) {
      return true;
    }
  }
  return false;
}

}  // namespace hw3