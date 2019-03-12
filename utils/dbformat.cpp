#include "dbformat.h"

namespace leveldb {

LookupKey::LookupKey(Slice& key, uint64_t seq_num) {
	char* dst = new char[key.size() + 13];
	start_ = dst;
	dst = EncodeVarint32(dst, key.size() + 8);
	key_start_ = dst;
	memcpy(dst, key.data(), key.size());
	EncodeFixed64(dst, (seq_num << 8) | 1);
	end_ = dst + 8;
}

}