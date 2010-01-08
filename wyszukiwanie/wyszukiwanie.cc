#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <set>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "double_metaphone.h"

typedef double number;

typedef std::pair<std::string, std::string> ss;  // String-string pair.
typedef std::pair<std::set<number>, std::set<ss> > parsed_t;  // Parsed line.
typedef std::pair<parsed_t, std::string> line_t;  // Parsed-raw pair.

char LINE_BREAK = '\n';

// Converts string to `T` using istringstream.
// Returns:
//   `true` if convertion suceeded,
//   `false` otherwise.
template <typename T>
bool string_to(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
}

// Round numbers point.
const number ROUND_POINT = 1000000000;

// Rounding function.
inline number round(number n)
{
    return static_cast<number>(ceil(n * ROUND_POINT)) / ROUND_POINT;
}

// Determines whether given numbers are similar.
inline bool numbers_similar(number n1, number n2)
{
    return n1 == n2;
}

// Determines wheter given sounds are similar
// based on similarity of one metaphone.
bool same_metaphone_or(const ss& p1, const ss& p2)
{
    return (p1.first == p2.first) || (p1.second == p2.second);
}

// Determines whether given sounds are similar
// based on similarity of both metaphones.
bool same_metaphone_and(const ss& p1, const ss& p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second);
}

// Dermines whether given lines are similar.
bool lines_similar(const parsed_t& line1, const parsed_t& line2)
{
    bool numbers_similar_found = line1.first.empty() && line2.first.empty();
    for(std::set<number>::const_iterator iter1 = line1.first.begin();
            iter1 != line1.first.end();
            iter1 ++) {
        if(line2.first.find(*iter1) != line2.first.end()) {
            numbers_similar_found = true;
            break;
        }
    }
    bool words_similar_found = line1.second.empty() && line2.second.empty();
    for(std::set<ss>::const_iterator iter1 = line1.second.begin();
            iter1 != line1.second.end();
            iter1 ++) {
        for(std::set<ss>::const_iterator iter2 = line2.second.begin();
                iter2 != line2.second.end();
                iter2 ++) {
            if(same_metaphone_and(*iter1, *iter2)) {
                words_similar_found = true;
                break;
            }
        }
        if(words_similar_found)
            break;
    }
    return numbers_similar_found && words_similar_found;
}

int main()
{
    // Split words in line with whitespaces: '\t' and ' '.
    boost::char_separator<char> whitespace_separator(" \t");
    std::multiset<line_t> input;
    // Read input.
    while(!std::cin.eof()) {
        std::string input_line;
        std::getline(std::cin, input_line);
        boost::tokenizer<boost::char_separator<char> >
            tokens(input_line, whitespace_separator);  // Tokenize line.
        parsed_t line = make_pair(std::set<number>(), std::set<ss>());
        // Parse line. Distribute numbers and non-numbers.
        BOOST_FOREACH(std::string s, tokens) {
            number tmp_num;
            if(string_to<number>(tmp_num, s, std::dec))
                line.first.insert(round(tmp_num));  // Push number.
            else
                line.second.insert(double_metaphone(s));  // Push non-number.
        }
        if(!(line.first.empty() && line.second.empty()))
            input.insert(make_pair(line, input_line));
    }
    std::cerr << std::flush;
    // Write result.
    std::multiset<line_t>::const_iterator iter_previous = input.begin();
    for(std::multiset<line_t>::const_iterator iter = input.begin();
            iter != input.end();
            iter ++) {
        if(!lines_similar(iter_previous->first, iter->first))
            std::cout << '\n';
        std::cout << iter->second << '\n';
        iter_previous = iter;
    }
    std::cout << std::flush;
    return 0;
}

