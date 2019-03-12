#include <algorithm>
#include <string>
#include "env.h"
#pragma once

namespace leveldb {
	
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

	bool ok() {
		return msg_ == "";
	}
	bool isNotFound() {
		return msg_ == sNotFound;
	}
	bool isError() {
		return msg_ == sError;
	}
private:
	std::string msg_;
};

}

