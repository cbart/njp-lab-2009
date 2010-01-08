#ifndef _WIDE_STRING_H_
#define _WIDE_STRING_H_

#include <iostream>
#include <string>

// Coverting UTF-8 string to wide string.
void unicode_to_wstring(std::wstring& dest, const std::string& src);

// Convert wide string to UTF-8 string.
void wstring_to_unicode(std::string& dest, const std::wstring& src);

// Printing wide strings to output stream.
std::ostream& operator<<(std::ostream& os, const std::wstring& ws);

// Getting wide strings from input stream.
std::istream& operator>>(std::istream& is, const std::wstring& ws);

#endif  // _WIDE_STRING_H_

