#include "utils/status.h"
#include "utils/env.h"
#include <vector>

namespace leveldb {
//struct Options;
struct FileMetaData;
//class Env;
//class Iterator;
//class TableCache;
class SkipList;
class VersionEdit;
class YangkvMain;
class Env;

// Build a Table file from the contents of *iter.  The generated file
// will be named according to meta->number.  On success, the rest of
// *meta will be filled with metadata about the generated table.
// If no data is present in *iter, meta->file_size will be set to
// zero, and no Table file will be produced.
Status BuildTable(std::vector<SkipList*>* frozenlists, Env* env, FileMetaData* meta);

}