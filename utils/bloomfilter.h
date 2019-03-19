#include <bits/stdc++.h>
using namespace std;
class BloomFilter {
public:
	void Init();
	void Recover();
	void Insert(const string&);
	bool Query(const string&);
	void close();
private:
	const static int kMaxSize = 1000000;
	const static int kFuncSize = 5;
	const string SaveFilePath = "./BloomFilter.txt";
	const vector<uint64_t>SeedSet = {131, 1919, 10007, 100000007, 1000000007};
	ifstream fin;
	ofstream fout;
	bool Slot_[kMaxSize];
};