#include <bits/stdc++.h>
#pragma once
using namespace std;

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
    WriterConfig(bool flag, int id, Writer* instance_) {
        stopFLAG = flag;
        instance = instance_;
        writerId = id;
    }
    Writer* instance;
    bool stopFLAG;
    int writerId;
};

class CompacterConfig{
public:
    CompacterConfig(){}
    CompacterConfig(bool flag, Compacter* instance_) {
        stopFLAG = flag;
        instance = instance_;
    }
    Compacter* instance;
    bool stopFLAG;
};
