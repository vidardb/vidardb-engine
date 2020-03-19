// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.

#include "vidardb/status.h"

namespace vidardb {

const char* Status::msgs[] = {
  "",                                                  // kNone
  "Timeout Acquiring Mutex",                           // kMutexTimeout
  "Timeout waiting to lock key",                       // kLockTimeout
  "Failed to acquire lock due to max_num_locks limit"  // kLockLimit
};

}  // namespace vidardb
