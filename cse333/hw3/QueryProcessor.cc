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

// Process additional words in the query (for multiword queries)
//
// Arguments:
// - itr_array: pointer to the array of IndexTableReaders
// - reslist: (output parameter) a list of IdxQueryResults, empty vector if no
// result is found
// - query: a vector of query words
// - idxfilenum: the current index file number (correspond to the index in the
// itr_array)
//
// Returns:
// - true if query results are found, false otherwise
static bool ProcessAdditionalWords(IndexTableReader** const itr_array,
                                   list<IdxQueryResult>* reslist,
                                   const vector<string>& query, int idxfilenum);

// Find the given filename in the final results vector
//
// Arguments:
// - final_results: a vector of QueryResults
// - filename: the filename the user is looking for
//
// Returns:
// - true if filename is found in final_result, false otherwise
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

  // Loop through and lookup the query in all the given index files
  for (i = 0; i < array_len_; i++) {
    // Look up the first word in the query
    didtr = itr_array_[i]->LookupWord(query[0]);
    if (didtr == nullptr) {
      // Continue to the next index file because the word is not found
      continue;
    }

    // We found the word, get a list of doc id element headers from the doc id
    // table reader
    idlist = didtr->GetDocIDList();

    // Push all the doc ids and their ranks from the doc id element headers to
    // the IdxQueryResult list
    for (DocIDElementHeader docid_header : idlist) {
      res.doc_id = docid_header.doc_id;
      res.rank = docid_header.num_positions;
      reslist.push_back(res);
    }

    // We are done using the doc id table reader, clean up its resources
    delete (didtr);

    // Check if we have more than one word in the query
    if (query.size() > 1) {
      // Process the additional words
      if (!ProcessAdditionalWords(itr_array_, &reslist, query, i)) {
        // No valid results found from the addtional words, which means there
        // are not matches in the current index file, continue to the next index
        // file.
        continue;
      }
    }

    // We are done processing the querys, now traslate the IdxQueryResults to
    // final QueryResults (using the doc table reader to find the filenames from
    // the doc ids)
    for (IdxQueryResult res : reslist) {
      string filename;

      // Find the filename using the doc table reader
      Verify333(dtr_array_[i]->LookupDocID(res.doc_id, &filename));

      // Check if the file name already exist in the final reasult (if the user
      // input the same index file twice this can prevent duplicates in the
      // final results)
      if (!FindFileName(final_result, filename)) {
        // If the filename is not in the final result, we add it to the final
        // result list
        QueryProcessor::QueryResult qres;
        qres.document_name = filename;
        qres.rank = res.rank;
        final_result.push_back(qres);
      }
    }

    // clear the IdxQueryResult list for reuse
    reslist.clear();
  }

  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

static bool ProcessAdditionalWords(IndexTableReader** const itr_array,
                                   list<IdxQueryResult>* reslist,
                                   const vector<string>& query,
                                   int idxfilenum) {
  list<DocIDElementHeader> idlist;
  list<DocPositionOffset_t> poslist;
  DocIDTableReader* didtr;
  uint32_t i, j, num_docs;

  // Loop through and process all the words left in the query
  for (i = 1; i < query.size(); i++) {
    // Look up the current word in the query
    didtr = itr_array[idxfilenum]->LookupWord(query[i]);
    if (didtr == nullptr) {
      // Clear the result list and return false because the word is not found
      reslist->clear();
      return false;
    }

    // We found the word, iterate through the docIDs in our current result list,
    // testing each to see whether it is also in the set of matches for the
    // query[i].
    //
    // If it is, we leave it in the result list and we update its rank by adding
    // in the number of matches for the current word.
    //
    // If it isn't, we delete that docID from the search result list.
    num_docs = reslist->size();
    list<IdxQueryResult>::iterator reslist_itr = reslist->begin();
    for (j = 0; j < num_docs; j++) {
      if (didtr->LookupDocID(reslist_itr->doc_id, &poslist)) {
        reslist_itr->rank += poslist.size();
        reslist_itr++;
      } else {
        reslist_itr = reslist->erase(reslist_itr);
      }
    }

    // We are done using the doc id table reader, clean up its resources
    delete (didtr);
  }

  // We've finished processing this current query word.  If there are no
  // documents left in our result list, return false.
  if (reslist->size() == 0) {
    return false;
  }

  return true;
}

static bool FindFileName(const vector<QueryProcessor::QueryResult> final_result,
                         const string& filename) {
  uint32_t i;

  // check the give filename agains each filenames in the final result and see
  // if there is a match
  for (i = 0; i < final_result.size(); i++) {
    if (final_result[i].document_name == filename) {
      return true;
    }
  }
  return false;
}

}  // namespace hw3
