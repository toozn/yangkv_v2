#include "/memory/skiplist.h"

using namespace std;

class Block() {
public:
	Block();
	~Block();
	size_t size() {
		return size_;
	}
private:
	uint32_t NumRestarts();
    uint32_t restart_offset_;
    const char* data_;
    size_t size_;
    bool owned_;
}
