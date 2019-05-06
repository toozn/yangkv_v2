#include <algorithm>
#include <string>
#pragma once

namespace yangkv {

static const std::string sNotFound = "NotFound";
static const std::string sError = "Error";
static const std::string sCorruption = "Corruption";
class Status {
public:
	Status() {
		msg_ = "";
	}

	Status(std::string msg) {
		msg_ = msg;
	} 

	static Status OK() {
		return Status();
	}

	static Status NotFound() {
		return Status(sNotFound);
	}

	static Status Error() {
		return Status(sError);
	}
	static Status Corruption() {
		return Status(sCorruption);
	}

	bool ok() {
		return msg_ == "";
	}
	bool isNotFound() {
		return msg_ == sNotFound;
	}
	bool isError() {
		return msg_ == sError;
	}
	bool isCorruption() {
		return msg_ == sCorruption;
	}
private:
	std::string msg_;
};

}

