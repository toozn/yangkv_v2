#include "slice.h"
#pragma once

namespace leveldb {

struct MemEntry {
	Slice key;
	Slice value;
	uint64_t seq_num;
	bool value_type;

	MemEntry(std::string& key_, std::string& value_, uint64_t seq, bool type) {
		key = Slice(key_);
		value = Slice(value_);
		seq_num = seq;
		value_type = type;
	}
};

Status DecodeMemEntry(Slice s, MemEntry& entry) {
	int klength, vlength;
	const char* key_ptr = GetVarint32Ptr(s, s + 5, &klength);
	entry.key = Slice(key_ptr, klength - 8);
	uint64_t info = DecodeFixed64(key_ptr + key_length - 8);
	entry.seq_num = info >> 8;
	entry.value_type = info & 255;
	entry.value = GetLengthPrefixedSlice(key_ptr + key_length);
	return Status::OK();
}

void EncodeMemEntry(MemEntry& entry, Slice s) {
	size_t key_size = key.size();
	size_t val_size = value.size();
	size_t internal_key_size = key_size + 8;
	const size_t encoded_len =
	VarintLength(internal_key_size) + internal_key_size + VarintLength(val_size) + val_size;
	char* buf = new char[encoded_len];
	char* p = EncodeVarint32(buf, internal_key_size);
	memcpy(p, key.data(), key_size);
	p += key_size;
	EncodeFixed64(p, (entry.seq_num << 8) | entry.value_type);
	p += 8;
	p = EncodeVarint32(p, val_size);
	memcpy(p, value.data(), val_size);
	assert(p + val_size == buf + encoded_len);
}

class LookupKey {
	LookupKey(Slice& key, uint64_t seq_num);

	Slice memtable_key() {
		return Slice(start_, end_ - start_);
	}
	Slice internal_key() {
		return Slice(kstart_, end_ - key_start_);
	}
	Slice user_key() {
		return Slice(kstart_, end_ - key_start_ - 8);
	}

	private:
	const char* start_;
	const char* end*;
	const char* key_start_;
};
}