#include "slice.h"
#include "coding.h"
#include <iostream>
#pragma once

namespace leveldb {

struct MemEntry {
	Slice key;
	Slice value;
	uint64_t seq_num;
	bool value_type;
	std::string* k;
	std::string* v;
	MemEntry() {}
	MemEntry(std::string& key_, std::string& value_, uint64_t seq, bool type) {
		k = new std::string(key_);
		v = new std::string(value_);
		key = Slice(*k);
		value = Slice(*v);
		seq_num = seq;
		value_type = type;
	}
	void debug() {
		printf("key:%s value:%s seq_num:%d value_type:%d\n",
        key.data(),
        value.data(),
        (int)seq_num,
        (int)value_type);
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