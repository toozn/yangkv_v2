#include <bits/stdc++.h>
#pragma once
using namespace std;

class PointCache{
public:
	void Insert(const string&, const string&);
	string Query(const string&);
	void init();
private:
	static const int kMaxSize = 10000;
	static const unsigned long long kSeed = 1145141919;
	pair<string, string> Slot_[kMaxSize];
}