#ifndef _INC_DBAURSHIM_GEHEIM
#define _INC_DBAURSHIM_GEHEIM

// this is secretive enough
static const char dbaurshim_secret[] = {0104, 0160, 0044};

#ifdef __cplusplus

#include <string>

// enterprise C++ means using namespaces
namespace dbaurshim {
	static const std::string secret(dbaurshim_secret);
}

#endif // __cplusplus

#endif // _INC_DBAURSHIM_GEHEIM