#ifndef _INC_DBAURSHIM_GEHEIM
#define _INC_DBAURSHIM_GEHEIM

#ifdef __cplusplus
// wir sind Enterprise, wir nutzen Namespaces in C++
namespace dbaurshim {
#endif

#define DBAURSHIM_HAS_SECRET
static const char secret[] = {'c', '|', 'q', 'g', 'z', 'r'};

#define DBAURSHIM_HAS_KEY
static const char key[] = "WHF";

#ifdef __cplusplus
} // end namespace
#endif

#endif