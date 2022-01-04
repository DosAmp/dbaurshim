#ifndef _INC_DBAURSHIM_GEHEIM
#define _INC_DBAURSHIM_GEHEIM

#define _DBAURSHIM_SECRET "c|qgzr"
#define _DBAURSHIM_KEY "WHF"

#ifdef __cplusplus

#include <string>

// wir sind Enterprise, wir nutzen Namespaces in C++
namespace dbaurshim {
	static const std::string secret(_DBAURSHIM_SECRET);
	static const std::string key(_DBAURSHIM_KEY);
}

#else

static const char dbaurshim_secret[] = _DBAURSHIM_SECRET;
static const char dbaurshim_key[] = _DBAURSHIM_KEY;

#endif // __cplusplus

#endif // _INC_DBAURSHIM_GEHEIM