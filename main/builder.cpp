#include "builder.h"
#include "utils/dbformat.h"
#include "memory/skiplist.h"
#include "versionedit.h"
#include "utils/env.h"
#include "utils/filename.h"
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
		return mg_it->second;
		mg_it++;
	}
	bool End() {
		return mg_it == mp_.end();
	}
private:
	std::map<std::string, Slice> mp_;
	std::map<std::string, Slice>::iterator mg_it;
};

Status BuildTable(std::vector<SkipList*>* frozenlists, Env* env,
					FileMetaData* meta) {
	assert(frozenlists != nullptr);
	MergeIterator* it = new MergeIterator(frozenlists);
	meta->file_size = 0;
	std::string fname = TableFileName(meta->number);
	PosixWritableFile* file;
    Status s = env->NewWritableFile(fname, &file);
	//TableBuilder* builder = new TableBuilder(options, file);
	int count = 0;
	MemEntry entry;
	while(!it->End()) {
		DecodeMemEntry(it->GetNext(), entry);
		count++;
		if(count % 100 == 0) entry.Debug();
	}
	delete it;
	return Status::OK();
}

}