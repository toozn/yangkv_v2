#include <bits/stdc++.h>
#pragma once

namespace yangkv {

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
    WriterConfig(int writerid, int logid) {
        stopFLAG = false;
        writerID = writerid;
        logID = logid;
    }
    bool stopFLAG;
    int writerID;
    int logID;
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