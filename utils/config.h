#include <bits/stdc++.h>
#pragma once

namespace leveldb {

class Compacter;
class Writer;

class Config{
public:
	Config(){}
	static const int kWriterNum = 8;
};

class WriterConfig{
public:
    WriterConfig(){}
    WriterConfig(int id) {
        stopFLAG = false;
        writerId = id;
    }
    bool stopFLAG;
    int writerId;
};

class CompacterConfig{
public:
    CompacterConfig() {
        stopFLAG = false;
    }
    Compacter* instance;
    bool stopFLAG;
};

}