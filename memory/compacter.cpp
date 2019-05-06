#include "compacter.h"
#include "main/builder.h"
#include "main/versionset.h"
#include <unistd.h>

namespace yangkv {

Status Compacter::compact(VersionEdit* edit) {
    assert(inCompact_ == true);
    Status s;
    FileMetaData metadata;
    metadata.number = ++file_seq_num_;
    s = BuildTable(set_->current_->list_, env_, &metadata);
    edit->AddFile(0, metadata.number, metadata.file_size,
                    metadata.smallest, metadata.largest);
    return s;
}

void Compacter::workRound() {
    printf("Compacter begin to work!\n");
    while (config_->stopFLAG == false) {
    	if (set_->current_->isFull()) {
    		inCompact_ = true;
    		Status s;
            VersionEdit edit;
            s = compact(&edit);
            Version* v = new Version(set_);
            v->apply(set_->current_, &edit);
    		set_->AppendVersion(v);
            set_->current_->debug();
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