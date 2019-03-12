#include "slice.h"
#include "coding.h"
#pragma once

namespace leveldb {

struct MemEntry {
	Slice key;
	Slice value;
	uint64_t seq_num;
	bool value_type;
	MemEntry() {}
	MemEntry(std::string& key_, std::string& value_, uint64_t seq, bool type) {
		key = Slice(key_);
		value = Slice(value_);
		seq_num = seq;
		value_type = type;
	}
};

Status DecodeMemEntry(Slice& s, MemEntry& entry);
void EncodeMemEntry(MemEntry& entry, Slice& s);

class LookupKey {
	LookupKey(Slice& key, uint64_t seq_num);

	Slice memtable_key() {
		return Slice(start_, end_ - start_);
	}
	Slice internal_key() {
		return Slice(key_start_, end_ - key_start_);
	}
	Slice user_key() {
		return Slice(key_start_, end_ - key_start_ - 8);
	}

	private:
	const char* start_;
	const char* end_;
	const char* key_start_;
};
}