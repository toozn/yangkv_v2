#include "compacter.h"
#include <unistd.h>
void* Compacter::compactRound(void* arg_) {
    auto arg = (CompacterConfig*)arg_;
    while (arg->stopFLAG == false) {
        pthread_mutex_lock(&lock_);
        pthread_cond_wait(&cond_, &lock_);
        CompactList();
        pthread_mutex_unlock(&lock_);
    }
    return 0;
}

void Compacter::pushList(SkipList* list) {
    pthread_mutex_lock(&lock_);
    list_[list_idx].push_back(list);
    if (list_[list_idx].size() >= kMaxFrozenListSize) {
        pthread_cond_signal(&cond_);
        list_idx = 1 - list_idx;
    }
    pthread_mutex_unlock(&lock_);
}

void Compacter::CompactList() {
   int sz = list_[list_idx].size();
   list_[list_idx].clear();
   //set_->CreateNewVersion();
}
