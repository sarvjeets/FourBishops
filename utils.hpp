// General purpose utils for logging, etc.

#ifndef cpp_utils_hpp
#define cpp_utils_hpp

#include <iostream>

// Use for generating log messages.
// E.g. LOG("Value of c is " << c)
#define LOG(x) cout << __FILE__ << ":" << __LINE__ << ":" << x << endl

#endif
