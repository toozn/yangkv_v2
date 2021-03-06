#include "utils/dbformat.h"
#include "utils/status.h"
#include <map>
#pragma once

namespace yangkv {

class MergeIterator;

class SkipList {
public:
    SkipList(){
        ref_ = 1;
        id_ = 0;
        list_.clear();
    };
    void ref() {
        ref_++;
    }
    void unref() {
        assert(ref_ >= 1);
        ref_--;
        if (ref_ == 0) {
            delete this;
        }
    }
    void insert(MemEntry& entry) {
        if (list_.size() == 0) {
            id_ = entry.seq_num;
        }
        Slice s;
        EncodeMemEntry(entry, s);
        list_[entry.key.ToString()] = s;
    }
    Status get(std::string& key, Slice& value) {
        if (list_.count(key) == 0) {
            return Status::NotFound();
        }
        value = list_[key];
        return Status::OK();
    }
    unsigned long long getId() {
        return id_;
    }
    unsigned int size() {
        return list_.size();
    }
    std::map<std::string, Slice>* get_list() {
        return &list_;
    }
    void Debug() {
        printf("size: %d\n", int(list_.size()));
        for (auto it: list_) {
            MemEntry entry;
            DecodeMemEntry(it.second, entry);
            entry.Debug();
        }
    }
private:
    unsigned long long id_;
    std::map<std::string, Slice>list_;
    int ref_;
};

}