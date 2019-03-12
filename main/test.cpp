#include "singleton.h"
#include <unistd.h>

using namespace leveldb;
using namespace std;

map<string, string>mp;
void* reader(void* arg) {
    auto db = Singleton::get();
    for (int i = 0; i <= 10000; i++) {
        string key = to_string(i);
        string value;
        Status s = db->getValue(key, &value);
        if (i % 2 == 0 && !s.isNotFound()) {
            printf("Error! %d\n", i);
        }
        if (i % 2 != 0 && value != mp[key]) {
            printf("Error! %s %s\n", value.c_str(), mp[key].c_str());
        }
   } 
   return nullptr;
}

int main() {
    auto db = Singleton::get();
    db->init();
    for (int i = 0; i <= 10000; i++) {
        string val = to_string(i);
        mp[val] = val;
        db->setKey(val, val);
        //usleep(5000);
    }
    for (int i = 0; i <= 10000; i += 2) {
        string val = to_string(i);
        mp[val] = "IsEmpty";
        db->delKey(val);
        //usleep(5000);
    }
    pthread_t pid[8];
    for (int i = 0; i < 8; i++) {
        pthread_create(&pid[i], NULL, reader, NULL);
    }
    for (int i = 0; i < 8; i++) {
        pthread_join(pid[i], NULL);
    }
    db->stop();
    sleep(2);
    printf("DB stoped\n");
}
