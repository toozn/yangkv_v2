#include "singleton.h"

pthread_once_t Singleton::once = PTHREAD_ONCE_INIT;
YangkvMain* Singleton::_instance = nullptr;
