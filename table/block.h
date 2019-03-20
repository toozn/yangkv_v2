#include <iostream>
#include <string>
#pragma once
namespace leveldb{

class BlockContents;
class Block {
public:
	Block(){};
	Block(const BlockContents& contents);
	~Block();
	size_t size() {
		return size_;
	}
private:
	uint32_t NumRestarts() const;
    uint32_t restart_offset_;
    const char* data_;
    size_t size_;
    bool owned_;
};

}
