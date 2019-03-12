#include "bloomfilter.h"

void BloomFilter::Init() {
	memset(Slot_, 0, sizeof(Slot_));
	fout.open(SaveFilePath);
}

void BloomFilter::Recover() {
	memset(Slot_, 0, sizeof(Slot_));
	fin.open(SaveFilePath);
	if (!fin) {
		fout.open(SaveFilePath, ios::app);
		return;
	}
	int FilledIdx = 0;
	while (fin >> FilledIdx) {
		assert(0 <= FilledIdx && FilledIdx < kMaxSize);
		Slot_[FilledIdx] = true;
	}
	fin.close();
	fout.open(SaveFilePath, ios::app);
}

void BloomFilter::Insert(const string& str) {
	if (str == "") {
		return;
	}

	for (const auto seed: SeedSet) {
		unsigned long long HashVal = 1;
		for (const char ch: str) {
			HashVal = (HashVal + (int)ch) * seed;
		}
		HashVal %= kMaxSize;
		if (Slot_[HashVal] == true) {
			continue;
		}
		else {
			Slot_[HashVal] = true;
			fout << " " << HashVal;
		}
	}
}

bool BloomFilter::Query(const string& str) {
	if (str == "") {
		return false;
	}
	for (const auto seed: SeedSet) {
		unsigned long long HashVal = 1;
		for (const char ch: str) {
			HashVal = (HashVal + (int)ch) * seed;
		}
		HashVal %= kMaxSize;
		if (Slot_[HashVal] == false) {
			return false;
		}
	}
	return true;
}

void BloomFilter::close() {
	fout.close();
}