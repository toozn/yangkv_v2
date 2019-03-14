#include "compacter.h"
#include <unistd.h>

namespace leveldb {

void* Compacter::compactRound(void* arg_) {
    auto arg = (CompacterConfig*)arg_;
    while (arg->stopFLAG == false) {
    	sleep(10);
    }
    return 0;
}

VersionEdit* calEdit(){
    return nullptr;
}

}