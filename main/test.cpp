#include "singleton.h"
#include "utils/dbformat.h"
#include "utils/coding.h"
#include <unistd.h>
#include <bits/stdc++.h>

using namespace leveldb;
using namespace std;

map<string, string>mp;

int size_ = 100;

void* reader(void* arg) {
    auto db = Singleton::get();
    for (int i = 0; i <= size_; i++) {
        string* key = new string(to_string(i));
        string value;
        Status s = db->getValue(*key, &value);
        if (i % 2 == 0 && !s.isNotFound()) {
            printf("Error! %d\n", i);
        }
        if (i % 2 != 0 && value != mp[*key]) {
            printf("Error! %s %s\n", value.c_str(), mp[*key].c_str());
        }
        delete key;
        //usleep(500);
   } 
   return nullptr;
}

void codingTEST() {
    for (int i = 0; i <= 30; i++) {
        string* key = new string(to_string(i));
        string* value = new string(to_string(i));
        MemEntry entry = MemEntry(*key, *value, i, i%2==0);
        MemEntry* result = new MemEntry();
        Slice s;
        EncodeMemEntry(entry, s);
        DecodeMemEntry(s, *result);
        result->Debug();
        delete key;
        delete value;
        delete result;
    }
    
}

/*
void mementryTEST() {
    vector<string*>ptrs;
    MemEntry* s[11];
    for (int i = 1; i <= 10; i++) {
        string* key = new string(to_string(i));
        string* value = new string(to_string(i));
        s[i] = new MemEntry(*key, *value, i, true);
        ptrs.push_back(key);
        ptrs.push_back(value);
    }
    for (int i = 1; i <= 10; i++) {
        s[i]->Debug();
    }
    int sz = ptrs.size();
    for (int i = 0; i < sz; i++) delete ptrs[i];
}
*/
int main() {
    codingTEST();
    vector<string*>ptrs;
    //mementryTEST();
    return 0;
    auto db = Singleton::get();
    db->init();
    for (int i = 0; i <= size_; i++) {
        string* key = new string(to_string(i));
        string* value = new string(to_string(i));
        mp[*key] = *value;
        db->setKey(*key, *value);
        ptrs.push_back(key);
        ptrs.push_back(value);
    }
    for (int i = 0; i <= size_; i += 2) {
        string* key = new string(to_string(i));
        mp[*key] = "IsEmpty";
        db->delKey(*key);
        ptrs.push_back(key);
    }
    
    pthread_t pid[8];
    for (int i = 0; i < 1; i++) {
        pthread_create(&pid[i], NULL, reader, NULL);
    }
    for (int i = 0; i < 1; i++) {
        pthread_join(pid[i], NULL);
    }
    db->stop();
    sleep(1);
    int sz = ptrs.size();
    for (int i = 0; i < sz; i++) delete ptrs[i];
    return 0;
    
}
