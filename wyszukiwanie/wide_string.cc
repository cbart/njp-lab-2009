#include <iostream>
#include <string>

#include "wide_string.h"

const char ERR = '?';
const wchar_t WIDE_ERR = L'�';

void unicode_to_wstring(std::wstring& dest, const std::string& src)
{
    dest.clear();
    wchar_t wide = 0;
    long bytes = 0;
    for(std::string::const_iterator iter = src.begin();
            iter != src.end();
            iter ++) {
        unsigned char c = static_cast<unsigned char>(*iter);
        if(c <= 0x7f) {
            if(bytes != 0) {
                dest.push_back(WIDE_ERR);
                bytes = 0;
            }
            dest.push_back(static_cast<wchar_t>(c));
        }
        else if(c <= 0xbf) {
            if(bytes != 0) {
                wide = ((wide << 6) | (c & 0x3f));
                bytes --;
                if(bytes == 0)
                    dest.push_back(wide);
            }
            else
                dest.push_back(WIDE_ERR);
        }
        else if(c <= 0xdf) {
            bytes = 1;
            wide = c & 0x1f;
        }
        else if(c <= 0xef) {
            bytes = 2;
            wide = c & 0x0f;
        }
        else if(c <= 0xf7) {
            bytes = 3;
            wide = c & 0x07;
        }
        else {
            dest.push_back(WIDE_ERR);
            bytes = 0;
        }
    }
    if(bytes != 0)
        dest.push_back(WIDE_ERR);
}

void wstring_to_unicode(std::string& dest, const std::wstring& src)
{
    dest.clear();
    for(std::wstring::const_iterator iter = src.begin();
            iter != src.end();
            iter ++) {
        wchar_t wide = *iter;
        if(wide <= 0x7f) {
            dest.push_back(static_cast<char>(wide));
        }
        else if(wide <= 0x7ff) {
            dest.push_back(0xc0 | ((wide >> 6) & 0x1f));
            dest.push_back(0x80 | (wide & 0x3f));
        }
        else if(wide <= 0xffff) {
            dest.push_back(0xe0 | ((wide >> 12) & 0x0f));
            dest.push_back(0x80 | ((wide >> 6) & 0x3f));
            dest.push_back(0x80 | (wide & 0x3f));
        }
        else if(wide <= 0x10ffff) {
            dest.push_back(0xf0 | ((wide >> 18) & 0x07));
            dest.push_back(0x80 | ((wide >> 12) & 0x3f));
            dest.push_back(0x80 | ((wide >> 6) & 0x3f));
            dest.push_back(0x80 | (wide & 0x3f));
        }
        else {
            dest.push_back(ERR);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const std::wstring& ws)
{
    std::string str;
    wstring_to_unicode(str, ws);
    os << str;
    return os;
}

std::istream& operator>>(std::istream& is, std::wstring& ws)
{
    std::string str;
    is >> str;
    unicode_to_wstring(ws, str);
    return is;
}


