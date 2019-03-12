#include <bits/stdc++.h>
#include "bloomfilter.h"
using namespace std;
int main() {
	
	BloomFilter bm;
	bm.Init();
	bm.Insert("abc");
	bm.Insert("def");
	bm.Insert("xyz");
	cout << bm.Query("abc") << endl;
	cout << bm.Query("def") << endl;
	cout << bm.Query("xyz") << endl;
	cout << bm.Query("123") << endl;
	cout << bm.Query("456") << endl;
	bm.close();
}