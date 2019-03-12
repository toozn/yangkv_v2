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

Status DecodeMemEntry(Slice s, MemEntry& entry) {
	uint32_t klength, vlength;
	const char* ptr = s.data();
	const char* key_ptr = GetVarint32Ptr(ptr, ptr + 5, &klength);
	entry.key = Slice(key_ptr, klength - 8);
	uint64_t info = DecodeFixed64(key_ptr + klength - 8);
	entry.seq_num = info >> 8;
	entry.value_type = info & 255;
	const char* vptr = key_ptr + klength;
	vptr = GetVarint32Ptr(vptr, vptr + 5, &vlength);
	entry.value = Slice(vptr, vlength);
	return Status::OK();
}

void EncodeMemEntry(MemEntry& entry, Slice s) {
	size_t key_size = entry.key.size();
	size_t val_size = entry.value.size();
	size_t internal_key_size = key_size + 8;
	const size_t encoded_len =
	VarintLength(internal_key_size) + internal_key_size + VarintLength(val_size) + val_size;
	char* buf = new char[encoded_len];
	char* p = EncodeVarint32(buf, internal_key_size);
	memcpy(p, entry.key.data(), key_size);
	p += key_size;
	EncodeFixed64(p, (entry.seq_num << 8) | entry.value_type);
	p += 8;
	p = EncodeVarint32(p, val_size);
	memcpy(p, entry.value.data(), val_size);
	assert(p + val_size == buf + encoded_len);
}

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