#ifndef _DOUBLE_METAPHONE_H_
#define _DOUBLE_METAPHONE_H_

#include <string>
#include <utility>

// Returns output of double mepaphone algorithm.
std::pair<std::string, std::string> double_metaphone(const std::string str);

#endif
