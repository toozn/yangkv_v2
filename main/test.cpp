#include "singleton.h"
#include "utils/dbformat.h"
#include "utils/coding.h"
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
        /*
        if (s.isNotFound()) {
            cout << i << " NotFound" << endl;
        }
        else {
            cout << i << " " << value << endl;
        }
        */
        
        if (i % 2 == 0 && !s.isNotFound()) {
            printf("Error! %d\n", i);
        }
        if (i % 2 != 0 && value != mp[key]) {
            printf("Error! %s %s\n", value.c_str(), mp[key].c_str());
        }
        
        //usleep(500);
   } 
   return nullptr;
}
/*
void test_coding() {
    string key = "123";
    string value = "456";
    MemEntry entry = MemEntry(&key, &value, 1, false);
    MemEntry result;
    Slice s;
    EncodeMemEntry(entry, s);
    DecodeMemEntry(s, result);
    std::cout << result.key.ToString() << std::endl;
    std::cout << result.value.ToString() << std::endl;
    std::cout << result.seq_num << std::endl;
    std::cout << result.value_type << std::endl;
}

void test_MemEntry() {
    MemEntry* arr[100];
    string str[100];
    for (int i = 0; i <= 100; i++) {
        str[i] = to_string(i);
    }
    for (int i = 0; i <= 10; i++) {
        arr[i] = new MemEntry(&str[i], &str[i], i, 0);
    }
    for (int i = 0; i <= 10; i++) {
        arr[i]->debug();
    }
}
*/
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
    sleep(1);
    for (int i = 0; i < 1; i++) {
        pthread_create(&pid[i], NULL, reader, NULL);
    }
    for (int i = 0; i < 1; i++) {
        pthread_join(pid[i], NULL);
    }
    db->stop();
    sleep(2);
    printf("DB stoped\n");
    
}
