//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.

#include "vidardb/snapshot.h"

#include "vidardb/db.h"

namespace vidardb {

ManagedSnapshot::ManagedSnapshot(DB* db) : db_(db),
                                           snapshot_(db->GetSnapshot()) {}

ManagedSnapshot::ManagedSnapshot(DB* db, const Snapshot* _snapshot)
    : db_(db), snapshot_(_snapshot) {}

ManagedSnapshot::~ManagedSnapshot() {
  if (snapshot_) {
    db_->ReleaseSnapshot(snapshot_);
  }
}

const Snapshot* ManagedSnapshot::snapshot() { return snapshot_;}

}  // namespace vidardb
