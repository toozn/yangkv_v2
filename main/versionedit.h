#include <string>
#pragma once
class VersionSet;
struct FileMetaData {
    int refs;
    std::string Lkey;
    std::string Hkey;
    int filesize;
    int filenumber;
    bool mayInFile(std::string& user_key){
    	return user_key >= Lkey && user_key <= Hkey;
    }
};
class VersionEdit{
public:
    void work();    
private:
    VersionSet* set_;
};
