#include "builder.h"
#include "utils/dbformat.h"
#include "memory/skiplist.h"
#include "versionedit.h"
#include "utils/env.h"
#include "utils/filename.h"
#include "table/table_builder.h"
#include <map>

namespace leveldb {

class MergeIterator {
public:
	MergeIterator(std::vector<SkipList*> frozenlists[]) {
		for (int i = 0; i < kMaxWriter; i++) {
			int list_size = frozenlists[i].size();
			for (int j = 0; j < list_size; j++) {
				auto list_ = frozenlists[i][j]->get_list();
				for (auto it: *list_) {
					mp_[it.first] = it.second;
				}
			}
		}
		mg_it = mp_.begin();
	}
	Slice& GetNext() {
		assert(mg_it != mp_.end());
		Slice& x = mg_it->second;
		mg_it++;
		return x;
	}
	bool End() {
		return mg_it == mp_.end();
	}
	void Debug() {
		for (auto it: mp_) {
			MemEntry entry;
			DecodeMemEntry(it.second, entry);
			printf("%s: ", it.first.c_str());
			entry.Debug();
		}
	}
private:
	std::map<std::string, Slice> mp_;
	std::map<std::string, Slice>::iterator mg_it;
};

Status BuildTable(std::vector<SkipList*>* frozenlists, Env* env,
					FileMetaData* meta) {
	assert(frozenlists != nullptr);
	meta->file_size = 0;
	std::string fname = TableFileName(meta->number);
	WritableFile* file;
    Status s = env->NewWritableFile(fname, &file);
    Options opt = Options();
	TableBuilder* builder = new TableBuilder(opt, file);
	MergeIterator iter(frozenlists);
	while(!iter.End()) {
		MemEntry entry;
		DecodeMemEntry(iter.GetNext(), entry);
		builder->Add(entry.key, entry.value);
	}
	s = builder->Finish();
    if (s.ok()) {
      meta->file_size = builder->FileSize();
      assert(meta->file_size > 0);
    }
    if (s.ok()) {
      s = file->Sync();
    }
    if (s.ok()) {
      s = file->Close();
    }
    delete file;
	printf("New File! %s\n", fname.c_str());
	return Status::OK();
}

}