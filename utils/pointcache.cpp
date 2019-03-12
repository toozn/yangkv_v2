#include "pointcache.h"

void PointCache::Init() {
	memset(Slot_, 0, sizeof(Slot_));
}

void PointCache::Insert(const string& key, const string& value) {
	unsigned long long HashVal = 1;
	for (const char ch: key) {
		HashVal = (HashVal + (int)ch) * kSeed;
	}
	HashVal %= kMaxSize;
	Slot_[HashVal] = {key, value};
}

string PointCache::Query(const string& key) {
	unsigned long long HashVal = 1;
	for (const char ch: key) {
		HashVal = (HashVal + (int)ch) * kSeed;
	}
	HashVal %= kMaxSize;
	if (Slot_[HashVal] != key) return "";
	return Slot_[HashVal];
}