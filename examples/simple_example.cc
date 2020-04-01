// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
#include <cstdio>
#include <string>
#include <iostream>
#include "vidardb/db.h"
#include "vidardb/slice.h"
#include "vidardb/options.h"
using namespace std;
using namespace vidardb;

std::string kDBPath = "/tmp/vidardb_simple_example";

int main() {
  DB* db;
  Options options;
  // Optimize VidarDB. This is the easiest way to get VidarDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  const Splitter* splitter = NewPipeSplitter();
  options.splitter = splitter;

  // open DB
  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());

  // Put key-value: key1 -> val11|val12
  s = db->Put(WriteOptions(), "key1", splitter->Stitch({"val11", "val12"}));
  assert(s.ok());

  // test memtable or sstable
  s = db->Flush(FlushOptions());
  assert(s.ok());

  // get value
  ReadOptions ro;
  ro.columns = {1};

  std::string value;
  s = db->Get(ro, "key1", &value);
  assert(s.ok());
  assert(value == "val11");
  cout << "key1: " << value << endl;

  // atomically apply a set of updates
  {
    WriteBatch batch;
    batch.Delete("key1");
    // key2 -> val21|val22
    batch.Put("key2", splitter->Stitch({"val21", "val22"}));
    s = db->Write(WriteOptions(), &batch);

    // test memtable or sstable
    s = db->Flush(FlushOptions());
    assert(s.ok());
  }

  s = db->Get(ro, "key1", &value);
  assert(s.IsNotFound());

  db->Get(ro, "key2", &value);
  assert(value == "val21");
  cout << "key2: " << value << endl;

  Iterator* iter = db->NewIterator(ro);
  for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
    cout << "key: " << iter->key().ToString()
         << " value: " << iter->value().ToString() << endl;
    ;
  }
  delete iter;

  delete db, splitter;

  return 0;
}
