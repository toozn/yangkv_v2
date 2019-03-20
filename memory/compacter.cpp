#include "compacter.h"
#include "main/builder.h"
#include "main/versionset.h"
#include <unistd.h>

namespace leveldb {

void Compacter::workRound() {
    printf("Compacter begin to work!\n");
    while (config_->stopFLAG == false) {
    	if (set_->current_->isFull()) {
    		inCompact_ = true;
    		FileMetaData metadata;
            metadata.number = ++file_seq_num_;
    		Status s;
    		s = BuildTable(set_->current_->list_, env_, &metadata);
    		set_->AppendVersion(new Version(set_));
            lock_->Boardcast();
    		inCompact_ = false;
    	}
    }
    printf("Quit Compacter!\n");
}

VersionEdit* calEdit(){
    return nullptr;
}

}