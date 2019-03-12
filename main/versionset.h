#include "memory/writer.h"
#include "memory/compacter.h"
#include "versionedit.h"
#pragma once
class VersionSet;
class yangkvMain;

class Version{
public:
    Version(){}
    Version(VersionSet* set) {
        set_ = set;
    }
    void ref();
    void unref();
    Status Get(Message& key);
    Status apply(Version* curr, VersionEdit* edit);
    Version* nxt_;
    Version* pre_;
private:
    friend class VersionSet;
    friend class VersionEdit;
    VersionSet* set_;
    int ref_;
    vector<SkipList*>maintainList_[kMaxWriter];
    vector<FileMetaData*>files_[kMaxLevel];
};

class VersionSet{
public:
    VersionSet(){
        dummyVersion_ = Version();
        current_ = &dummyVersion_;
    }
    void addVersion(Version* v);
    Status applyEdit(VersionEdit* edit);
private:
    friend class yangkvMain;
    friend class Version;
    Version* current_;
    Version dummyVersion_;
    yangkvMain* db_;
private:
};
