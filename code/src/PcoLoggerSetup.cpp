#include "pcosynchro/pcologger.h"

int PcoLogger::sm_verbosity = 0;
std::mutex PcoLogger::sm_mutex;