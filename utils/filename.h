#include <string>
#include <cstring>
#pragma once
namespace yangkv {

std::string TableFileName(uint64_t file_number) {
	return "yang_sst_" + std::to_string(file_number);
}



}