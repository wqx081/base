#ifndef KUDU_LOG_LOG_READER_H_
#define KUDU_LOG_LOG_READER_H_

#include <gtest/gtest.h>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/raft/log_metrics.h"
#include "base/raft/log_util.h"
#include "base/raft/opid_util.h"
#include "base/fs/fs_manager.h"
#include "base/core/ref_counted.h"
#include "base/core/spinlock.h"
#include "base/util/make_shared.h"
#include "base/util/locks.h"

namespace base {
namespace log {
class Log;
class LogIndex;
struct LogIndexEntry;

// Reads a set of segments from a given path. Segment headers and footers
// are read and parsed, but entries are not.
// This class is thread safe.
class LogReader {
 public:
  ~LogReader();

  // Opens a LogReader on the default tablet log directory, and sets
  // 'reader' to the newly created LogReader.
  //
  // 'index' may be NULL, but if it is, ReadReplicatesInRange() may not
  // be used.
  static Status Open(FsManager* fs_manager,
                     const scoped_refptr<LogIndex>& index,
                     const std::string& tablet_id,
                     const scoped_refptr<MetricEntity>& metric_entity,
                     std::shared_ptr<LogReader>* reader);

  // Opens a LogReader on a specific tablet log recovery directory, and sets
  // 'reader' to the newly created LogReader.
  static Status OpenFromRecoveryDir(FsManager* fs_manager,
                                    const std::string& tablet_id,
                                    const scoped_refptr<MetricEntity>& metric_entity,
                                    std::shared_ptr<LogReader>* reader);

  // Return the minimum replicate index that is retained in the currently available
  // logs. May return -1 if no replicates have been logged.
  int64_t GetMinReplicateIndex() const;

  // Return a readable segment with the given sequence number, or NULL if it
  // cannot be found (e.g. if it has already been GCed).
  scoped_refptr<ReadableLogSegment> GetSegmentBySequenceNumber(int64_t seq) const;

  // Copies a snapshot of the current sequence of segments into 'segments'.
  // 'segments' will be cleared first.
  Status GetSegmentsSnapshot(SegmentSequence* segments) const;

  // Reads all ReplicateMsgs from 'starting_at' to 'up_to' both inclusive.
  // The caller takes ownership of the returned ReplicateMsg objects.
  //
  // Will attempt to read no more than 'max_bytes_to_read', unless it is set to
  // LogReader::kNoSizeLimit. If the size limit would prevent reading any operations at
  // all, then will read exactly one operation.
  //
  // Requires that a LogIndex was passed into LogReader::Open().
  Status ReadReplicatesInRange(
      int64_t starting_at,
      int64_t up_to,
      int64_t max_bytes_to_read,
      std::vector<consensus::ReplicateMsg*>* replicates) const;
  static const int kNoSizeLimit;

  // Look up the OpId for the given operation index.
  // Returns a bad Status if the log index fails to load (eg. due to an IO error).
  Status LookupOpId(int64_t op_index, consensus::OpId* op_id) const;

  // Returns the number of segments.
  const int num_segments() const;

  std::string ToString() const;

 private:
  FRIEND_TEST(LogTest, TestLogReader);
  FRIEND_TEST(LogTest, TestReadLogWithReplacedReplicates);
  friend class Log;
  friend class LogTest;

  enum State {
    kLogReaderInitialized,
    kLogReaderReading,
    kLogReaderClosed
  };

  // Appends 'segment' to the segments available for read by this reader.
  // Index entries in 'segment's footer will be added to the index.
  // If the segment has no footer it will be scanned so this should not be used
  // for new segments.
  Status AppendSegment(const scoped_refptr<ReadableLogSegment>& segment);

  // Same as above but for segments without any entries.
  // Used by the Log to add "empty" segments.
  Status AppendEmptySegment(const scoped_refptr<ReadableLogSegment>& segment);

  // Removes segments with sequence numbers less than or equal to
  // 'segment_sequence_number' from this reader.
  Status TrimSegmentsUpToAndIncluding(int64_t segment_sequence_number);

  // Replaces the last segment in the reader with 'segment'.
  // Used to replace a segment that was still in the process of being written
  // with its complete version which has a footer and index entries.
  // Requires that the last segment in 'segments_' has the same sequence
  // number as 'segment'.
  // Expects 'segment' to be properly closed and to have footer.
  Status ReplaceLastSegment(const scoped_refptr<ReadableLogSegment>& segment);

  // Appends 'segment' to the segment sequence.
  // Assumes that the segment was scanned, if no footer was found.
  // To be used only internally, clients of this class with private access (i.e. friends)
  // should use the thread safe version, AppendSegment(), which will also scan the segment
  // if no footer is present.
  Status AppendSegmentUnlocked(const scoped_refptr<ReadableLogSegment>& segment);

  // Used by Log to update its LogReader on how far it is possible to read
  // the current segment. Requires that the reader has at least one segment
  // and that the last segment has no footer, meaning it is currently being
  // written to.
  void UpdateLastSegmentOffset(int64_t readable_to_offset);

  // Read the LogEntryBatchPB pointed to by the provided index entry.
  // 'tmp_buf' is used as scratch space to avoid extra allocation.
  Status ReadBatchUsingIndexEntry(const LogIndexEntry& index_entry,
                                  faststring* tmp_buf,
                                  gscoped_ptr<LogEntryBatchPB>* batch) const;

  LogReader(FsManager* fs_manager, const scoped_refptr<LogIndex>& index,
            std::string tablet_id,
            const scoped_refptr<MetricEntity>& metric_entity);

  // Reads the headers of all segments in 'tablet_wal_path'.
  Status Init(const std::string& tablet_wal_path);

  // Initializes an 'empty' reader for tests, i.e. does not scan a path looking for segments.
  Status InitEmptyReaderForTests();

  FsManager *fs_manager_;
  const scoped_refptr<LogIndex> log_index_;
  const std::string tablet_id_;

  // Metrics
  scoped_refptr<Counter> bytes_read_;
  scoped_refptr<Counter> entries_read_;
  scoped_refptr<Histogram> read_batch_latency_;

  // The sequence of all current log segments in increasing sequence number
  // order.
  SegmentSequence segments_;

  mutable simple_spinlock lock_;

  State state_;

  ALLOW_MAKE_SHARED(LogReader);
  DISALLOW_COPY_AND_ASSIGN(LogReader);
};

}  // namespace log
}  // namespace base

#endif /* KUDU_LOG_LOG_READER_H_ */
