// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.

#ifndef STORAGE_VIDARDB_INCLUDE_STATISTICS_H_
#define STORAGE_VIDARDB_INCLUDE_STATISTICS_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace vidardb {

/**
 * Keep adding ticker's here.
 *  1. Any ticker should be added before TICKER_ENUM_MAX.
 *  2. Add a readable string in TickersNameMap below for the newly added ticker.
 */
enum Tickers : uint32_t {
  // total block cache misses
  // REQUIRES: BLOCK_CACHE_MISS == BLOCK_CACHE_INDEX_MISS +
  //                               BLOCK_CACHE_FILTER_MISS +
  //                               BLOCK_CACHE_DATA_MISS;
  BLOCK_CACHE_MISS = 0,
  // total block cache hit
  // REQUIRES: BLOCK_CACHE_HIT == BLOCK_CACHE_INDEX_HIT +
  //                              BLOCK_CACHE_FILTER_HIT +
  //                              BLOCK_CACHE_DATA_HIT;
  BLOCK_CACHE_HIT,
  // # of blocks added to block cache.
  BLOCK_CACHE_ADD,
  // # of failures when adding blocks to block cache.
  BLOCK_CACHE_ADD_FAILURES,
  // # of times cache miss when accessing index block from block cache.
  BLOCK_CACHE_INDEX_MISS,
  // # of times cache hit when accessing index block from block cache.
  BLOCK_CACHE_INDEX_HIT,
  // # of bytes of index blocks inserted into cache
  BLOCK_CACHE_INDEX_BYTES_INSERT,
  // # of bytes of index block erased from cache
  BLOCK_CACHE_INDEX_BYTES_EVICT,
  // # of times cache miss when accessing filter block from block cache.
  BLOCK_CACHE_FILTER_MISS,
  // # of times cache hit when accessing filter block from block cache.
  BLOCK_CACHE_FILTER_HIT,
  // # of bytes of bloom filter blocks inserted into cache
  BLOCK_CACHE_FILTER_BYTES_INSERT,
  // # of bytes of bloom filter block erased from cache
  BLOCK_CACHE_FILTER_BYTES_EVICT,
  // # of times cache miss when accessing data block from block cache.
  BLOCK_CACHE_DATA_MISS,
  // # of times cache hit when accessing data block from block cache.
  BLOCK_CACHE_DATA_HIT,
  // # of bytes read from cache.
  BLOCK_CACHE_BYTES_READ,
  // # of bytes written into cache.
  BLOCK_CACHE_BYTES_WRITE,

  // # of times bloom filter has avoided file reads.
  BLOOM_FILTER_USEFUL,

  // # persistent cache hit
  PERSISTENT_CACHE_HIT,
  // # persistent cache miss
  PERSISTENT_CACHE_MISS,

  // # of memtable hits.
  MEMTABLE_HIT,
  // # of memtable misses.
  MEMTABLE_MISS,

  // # of Get() queries served by L0
  GET_HIT_L0,
  // # of Get() queries served by L1
  GET_HIT_L1,
  // # of Get() queries served by L2 and up
  GET_HIT_L2_AND_UP,

  /**
   * COMPACTION_KEY_DROP_* count the reasons for key drop during compaction
   * There are 3 reasons currently.
   */
  COMPACTION_KEY_DROP_NEWER_ENTRY,  // key was written with a newer value.
  COMPACTION_KEY_DROP_OBSOLETE,     // The key is obsolete.
  COMPACTION_KEY_DROP_USER,  // user compaction function has dropped the key.

  // Number of keys written to the database via the Put and Write call's
  NUMBER_KEYS_WRITTEN,
  // Number of Keys read,
  NUMBER_KEYS_READ,
  // Number keys updated, if inplace update is enabled
  NUMBER_KEYS_UPDATED,
  // The number of uncompressed bytes issued by DB::Put(), DB::Delete(),
  // DB::Merge(), and DB::Write().
  BYTES_WRITTEN,
  // The number of uncompressed bytes read from DB::Get().  It could be
  // either from memtables, cache, or table files.
  // For the number of logical bytes read from DB::MultiGet(),
  // please use NUMBER_MULTIGET_BYTES_READ.
  BYTES_READ,
  // The number of calls to seek/next/prev
  NUMBER_DB_SEEK,
  NUMBER_DB_NEXT,
  NUMBER_DB_PREV,
  // The number of calls to seek/next/prev that returned data
  NUMBER_DB_SEEK_FOUND,
  NUMBER_DB_NEXT_FOUND,
  NUMBER_DB_PREV_FOUND,
  // The number of uncompressed bytes read from an iterator.
  // Includes size of key and value.
  ITER_BYTES_READ,
  NO_FILE_CLOSES,
  NO_FILE_OPENS,
  NO_FILE_ERRORS,
  // DEPRECATED Time system had to wait to do LO-L1 compactions
  STALL_L0_SLOWDOWN_MICROS,
  // DEPRECATED Time system had to wait to move memtable to L1.
  STALL_MEMTABLE_COMPACTION_MICROS,
  // DEPRECATED write throttle because of too many files in L0
  STALL_L0_NUM_FILES_MICROS,
  // Writer has to wait for compaction or flush to finish.
  STALL_MICROS,
  // The wait time for db mutex.
  // Disabled by default. To enable it set stats level to kAll
  DB_MUTEX_WAIT_MICROS,
  RATE_LIMIT_DELAY_MILLIS,
  NO_ITERATORS,  // number of iterators currently open

  // Number of MultiGet calls, keys read, and bytes read
  NUMBER_MULTIGET_CALLS,
  NUMBER_MULTIGET_KEYS_READ,
  NUMBER_MULTIGET_BYTES_READ,

  // Number of deletes records that were not required to be
  // written to storage because key does not exist
  NUMBER_FILTERED_DELETES,
  NUMBER_MERGE_FAILURES,
  SEQUENCE_NUMBER,

  // number of times bloom was checked before creating iterator on a
  // file, and the number of times the check was useful in avoiding
  // iterator creation (and thus likely IOPs).
  BLOOM_FILTER_PREFIX_CHECKED,
  BLOOM_FILTER_PREFIX_USEFUL,

  // Record the number of calls to GetUpadtesSince. Useful to keep track of
  // transaction log iterator refreshes
  GET_UPDATES_SINCE_CALLS,
  BLOCK_CACHE_COMPRESSED_MISS,  // miss in the compressed block cache
  BLOCK_CACHE_COMPRESSED_HIT,   // hit in the compressed block cache
  // Number of blocks added to comopressed block cache
  BLOCK_CACHE_COMPRESSED_ADD,
  // Number of failures when adding blocks to compressed block cache
  BLOCK_CACHE_COMPRESSED_ADD_FAILURES,
  WAL_FILE_SYNCED,  // Number of times WAL sync is done
  WAL_FILE_BYTES,   // Number of bytes written to WAL

  // Writes can be processed by requesting thread or by the thread at the
  // head of the writers queue.
  WRITE_DONE_BY_SELF,
  WRITE_DONE_BY_OTHER,  // Equivalent to writes done for others
  WRITE_TIMEDOUT,       // Number of writes ending up with timed-out.
  WRITE_WITH_WAL,       // Number of Write calls that request WAL
  COMPACT_READ_BYTES,   // Bytes read during compaction
  COMPACT_WRITE_BYTES,  // Bytes written during compaction
  FLUSH_WRITE_BYTES,    // Bytes written during flush

  // Number of table's properties loaded directly from file, without creating
  // table reader object.
  NUMBER_DIRECT_LOAD_TABLE_PROPERTIES,
  NUMBER_SUPERVERSION_ACQUIRES,
  NUMBER_SUPERVERSION_RELEASES,
  NUMBER_SUPERVERSION_CLEANUPS,
  NUMBER_BLOCK_NOT_COMPRESSED,
  MERGE_OPERATION_TOTAL_TIME,
  FILTER_OPERATION_TOTAL_TIME,

  // Row cache.
  ROW_CACHE_HIT,
  ROW_CACHE_MISS,

  TICKER_ENUM_MAX
};

// The order of items listed in  Tickers should be the same as
// the order listed in TickersNameMap
const std::vector<std::pair<Tickers, std::string>> TickersNameMap = {
    {BLOCK_CACHE_MISS, "vidardb.block.cache.miss"},
    {BLOCK_CACHE_HIT, "vidardb.block.cache.hit"},
    {BLOCK_CACHE_ADD, "vidardb.block.cache.add"},
    {BLOCK_CACHE_ADD_FAILURES, "vidardb.block.cache.add.failures"},
    {BLOCK_CACHE_INDEX_MISS, "vidardb.block.cache.index.miss"},
    {BLOCK_CACHE_INDEX_HIT, "vidardb.block.cache.index.hit"},
    {BLOCK_CACHE_INDEX_BYTES_INSERT, "vidardb.block.cache.index.bytes.insert"},
    {BLOCK_CACHE_INDEX_BYTES_EVICT, "vidardb.block.cache.index.bytes.evict"},
    {BLOCK_CACHE_FILTER_MISS, "vidardb.block.cache.filter.miss"},
    {BLOCK_CACHE_FILTER_HIT, "vidardb.block.cache.filter.hit"},
    {BLOCK_CACHE_FILTER_BYTES_INSERT,
     "vidardb.block.cache.filter.bytes.insert"},
    {BLOCK_CACHE_FILTER_BYTES_EVICT, "vidardb.block.cache.filter.bytes.evict"},
    {BLOCK_CACHE_DATA_MISS, "vidardb.block.cache.data.miss"},
    {BLOCK_CACHE_DATA_HIT, "vidardb.block.cache.data.hit"},
    {BLOCK_CACHE_BYTES_READ, "vidardb.block.cache.bytes.read"},
    {BLOCK_CACHE_BYTES_WRITE, "vidardb.block.cache.bytes.write"},
    {BLOOM_FILTER_USEFUL, "vidardb.bloom.filter.useful"},
    {MEMTABLE_HIT, "vidardb.memtable.hit"},
    {MEMTABLE_MISS, "vidardb.memtable.miss"},
    {GET_HIT_L0, "vidardb.l0.hit"},
    {GET_HIT_L1, "vidardb.l1.hit"},
    {GET_HIT_L2_AND_UP, "vidardb.l2andup.hit"},
    {COMPACTION_KEY_DROP_NEWER_ENTRY, "vidardb.compaction.key.drop.new"},
    {COMPACTION_KEY_DROP_OBSOLETE, "vidardb.compaction.key.drop.obsolete"},
    {COMPACTION_KEY_DROP_USER, "vidardb.compaction.key.drop.user"},
    {NUMBER_KEYS_WRITTEN, "vidardb.number.keys.written"},
    {NUMBER_KEYS_READ, "vidardb.number.keys.read"},
    {NUMBER_KEYS_UPDATED, "vidardb.number.keys.updated"},
    {BYTES_WRITTEN, "vidardb.bytes.written"},
    {BYTES_READ, "vidardb.bytes.read"},
    {NUMBER_DB_SEEK, "vidardb.number.db.seek"},
    {NUMBER_DB_NEXT, "vidardb.number.db.next"},
    {NUMBER_DB_PREV, "vidardb.number.db.prev"},
    {NUMBER_DB_SEEK_FOUND, "vidardb.number.db.seek.found"},
    {NUMBER_DB_NEXT_FOUND, "vidardb.number.db.next.found"},
    {NUMBER_DB_PREV_FOUND, "vidardb.number.db.prev.found"},
    {ITER_BYTES_READ, "vidardb.db.iter.bytes.read"},
    {NO_FILE_CLOSES, "vidardb.no.file.closes"},
    {NO_FILE_OPENS, "vidardb.no.file.opens"},
    {NO_FILE_ERRORS, "vidardb.no.file.errors"},
    {STALL_L0_SLOWDOWN_MICROS, "vidardb.l0.slowdown.micros"},
    {STALL_MEMTABLE_COMPACTION_MICROS, "vidardb.memtable.compaction.micros"},
    {STALL_L0_NUM_FILES_MICROS, "vidardb.l0.num.files.stall.micros"},
    {STALL_MICROS, "vidardb.stall.micros"},
    {DB_MUTEX_WAIT_MICROS, "vidardb.db.mutex.wait.micros"},
    {RATE_LIMIT_DELAY_MILLIS, "vidardb.rate.limit.delay.millis"},
    {NO_ITERATORS, "vidardb.num.iterators"},
    {NUMBER_MULTIGET_CALLS, "vidardb.number.multiget.get"},
    {NUMBER_MULTIGET_KEYS_READ, "vidardb.number.multiget.keys.read"},
    {NUMBER_MULTIGET_BYTES_READ, "vidardb.number.multiget.bytes.read"},
    {NUMBER_FILTERED_DELETES, "vidardb.number.deletes.filtered"},
    {NUMBER_MERGE_FAILURES, "vidardb.number.merge.failures"},
    {SEQUENCE_NUMBER, "vidardb.sequence.number"},
    {BLOOM_FILTER_PREFIX_CHECKED, "vidardb.bloom.filter.prefix.checked"},
    {BLOOM_FILTER_PREFIX_USEFUL, "vidardb.bloom.filter.prefix.useful"},
    {GET_UPDATES_SINCE_CALLS, "vidardb.getupdatessince.calls"},
    {BLOCK_CACHE_COMPRESSED_MISS, "vidardb.block.cachecompressed.miss"},
    {BLOCK_CACHE_COMPRESSED_HIT, "vidardb.block.cachecompressed.hit"},
    {BLOCK_CACHE_COMPRESSED_ADD, "vidardb.block.cachecompressed.add"},
    {BLOCK_CACHE_COMPRESSED_ADD_FAILURES,
     "vidardb.block.cachecompressed.add.failures"},
    {WAL_FILE_SYNCED, "vidardb.wal.synced"},
    {WAL_FILE_BYTES, "vidardb.wal.bytes"},
    {WRITE_DONE_BY_SELF, "vidardb.write.self"},
    {WRITE_DONE_BY_OTHER, "vidardb.write.other"},
    {WRITE_WITH_WAL, "vidardb.write.wal"},
    {FLUSH_WRITE_BYTES, "vidardb.flush.write.bytes"},
    {COMPACT_READ_BYTES, "vidardb.compact.read.bytes"},
    {COMPACT_WRITE_BYTES, "vidardb.compact.write.bytes"},
    {NUMBER_DIRECT_LOAD_TABLE_PROPERTIES,
     "vidardb.number.direct.load.table.properties"},
    {NUMBER_SUPERVERSION_ACQUIRES, "vidardb.number.superversion_acquires"},
    {NUMBER_SUPERVERSION_RELEASES, "vidardb.number.superversion_releases"},
    {NUMBER_SUPERVERSION_CLEANUPS, "vidardb.number.superversion_cleanups"},
    {NUMBER_BLOCK_NOT_COMPRESSED, "vidardb.number.block.not_compressed"},
    {MERGE_OPERATION_TOTAL_TIME, "vidardb.merge.operation.time.nanos"},
    {FILTER_OPERATION_TOTAL_TIME, "vidardb.filter.operation.time.nanos"},
    {ROW_CACHE_HIT, "vidardb.row.cache.hit"},
    {ROW_CACHE_MISS, "vidardb.row.cache.miss"},
};

/**
 * Keep adding histogram's here.
 * Any histogram whould have value less than HISTOGRAM_ENUM_MAX
 * Add a new Histogram by assigning it the current value of HISTOGRAM_ENUM_MAX
 * Add a string representation in HistogramsNameMap below
 * And increment HISTOGRAM_ENUM_MAX
 */
enum Histograms : uint32_t {
  DB_GET = 0,
  DB_WRITE,
  COMPACTION_TIME,
  SUBCOMPACTION_SETUP_TIME,
  TABLE_SYNC_MICROS,
  COMPACTION_OUTFILE_SYNC_MICROS,
  WAL_FILE_SYNC_MICROS,
  MANIFEST_FILE_SYNC_MICROS,
  // TIME SPENT IN IO DURING TABLE OPEN
  TABLE_OPEN_IO_MICROS,
  DB_MULTIGET,
  READ_BLOCK_COMPACTION_MICROS,
  READ_BLOCK_GET_MICROS,
  WRITE_RAW_BLOCK_MICROS,
  STALL_L0_SLOWDOWN_COUNT,
  STALL_MEMTABLE_COMPACTION_COUNT,
  STALL_L0_NUM_FILES_COUNT,
  HARD_RATE_LIMIT_DELAY_COUNT,
  SOFT_RATE_LIMIT_DELAY_COUNT,
  NUM_FILES_IN_SINGLE_COMPACTION,
  DB_SEEK,
  WRITE_STALL,
  SST_READ_MICROS,
  // The number of subcompactions actually scheduled during a compaction
  NUM_SUBCOMPACTIONS_SCHEDULED,
  // Value size distribution in each operation
  BYTES_PER_READ,
  BYTES_PER_WRITE,
  BYTES_PER_MULTIGET,
  HISTOGRAM_ENUM_MAX,  // TODO(ldemailly): enforce HistogramsNameMap match
};

const std::vector<std::pair<Histograms, std::string>> HistogramsNameMap = {
    {DB_GET, "vidardb.db.get.micros"},
    {DB_WRITE, "vidardb.db.write.micros"},
    {COMPACTION_TIME, "vidardb.compaction.times.micros"},
    {SUBCOMPACTION_SETUP_TIME, "vidardb.subcompaction.setup.times.micros"},
    {TABLE_SYNC_MICROS, "vidardb.table.sync.micros"},
    {COMPACTION_OUTFILE_SYNC_MICROS, "vidardb.compaction.outfile.sync.micros"},
    {WAL_FILE_SYNC_MICROS, "vidardb.wal.file.sync.micros"},
    {MANIFEST_FILE_SYNC_MICROS, "vidardb.manifest.file.sync.micros"},
    {TABLE_OPEN_IO_MICROS, "vidardb.table.open.io.micros"},
    {DB_MULTIGET, "vidardb.db.multiget.micros"},
    {READ_BLOCK_COMPACTION_MICROS, "vidardb.read.block.compaction.micros"},
    {READ_BLOCK_GET_MICROS, "vidardb.read.block.get.micros"},
    {WRITE_RAW_BLOCK_MICROS, "vidardb.write.raw.block.micros"},
    {STALL_L0_SLOWDOWN_COUNT, "vidardb.l0.slowdown.count"},
    {STALL_MEMTABLE_COMPACTION_COUNT, "vidardb.memtable.compaction.count"},
    {STALL_L0_NUM_FILES_COUNT, "vidardb.num.files.stall.count"},
    {HARD_RATE_LIMIT_DELAY_COUNT, "vidardb.hard.rate.limit.delay.count"},
    {SOFT_RATE_LIMIT_DELAY_COUNT, "vidardb.soft.rate.limit.delay.count"},
    {NUM_FILES_IN_SINGLE_COMPACTION, "vidardb.numfiles.in.singlecompaction"},
    {DB_SEEK, "vidardb.db.seek.micros"},
    {WRITE_STALL, "vidardb.db.write.stall"},
    {SST_READ_MICROS, "vidardb.sst.read.micros"},
    {NUM_SUBCOMPACTIONS_SCHEDULED, "vidardb.num.subcompactions.scheduled"},
    {BYTES_PER_READ, "vidardb.bytes.per.read"},
    {BYTES_PER_WRITE, "vidardb.bytes.per.write"},
    {BYTES_PER_MULTIGET, "vidardb.bytes.per.multiget"},
};

struct HistogramData {
  double median;
  double percentile95;
  double percentile99;
  double average;
  double standard_deviation;
};

enum StatsLevel {
  // Collect all stats except the counters requiring to get time inside the
  // mutex lock.
  kExceptTimeForMutex,
  // Collect all stats, including measuring duration of mutex operations.
  // If getting time is expensive on the platform to run, it can
  // reduce scalability to more threads, especialy for writes.
  kAll,
};

// Analyze the performance of a db
class Statistics {
 public:
  virtual ~Statistics() {}

  virtual uint64_t getTickerCount(uint32_t tickerType) const = 0;
  virtual void histogramData(uint32_t type,
                             HistogramData* const data) const = 0;
  virtual std::string getHistogramString(uint32_t type) const { return ""; }
  virtual void recordTick(uint32_t tickerType, uint64_t count = 0) = 0;
  virtual void setTickerCount(uint32_t tickerType, uint64_t count) = 0;
  virtual void measureTime(uint32_t histogramType, uint64_t time) = 0;

  // String representation of the statistic object.
  virtual std::string ToString() const {
    // Do nothing by default
    return std::string("ToString(): not implemented");
  }

  // Override this function to disable particular histogram collection
  virtual bool HistEnabledForType(uint32_t type) const {
    return type < HISTOGRAM_ENUM_MAX;
  }

  StatsLevel stats_level_ = kExceptTimeForMutex;
};

// Create a concrete DBStatistics object
std::shared_ptr<Statistics> CreateDBStatistics();

}  // namespace vidardb

#endif  // STORAGE_VIDARDB_INCLUDE_STATISTICS_H_
