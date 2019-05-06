#include "utils/status.h"
#include "utils/env.h"
#include <vector>

namespace yangkv {
//struct Options;
struct FileMetaData;
//class Env;
//class Iterator;
//class TableCache;
class SkipList;
class VersionEdit;
class YangkvMain;
class Env;

Status BuildTable(std::vector<SkipList*>* frozenlists, Env* env, FileMetaData* meta);

}