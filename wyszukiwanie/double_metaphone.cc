// Double Metaphone - based on the C++ implementation by Lawrence Philips.

#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cassert>
#include <utility>
#include <string>

#include "double_metaphone.h"

typedef struct metastring
{
    char *str;
    int length;
    int bufsize;
    int free_string_on_destroy;
} metastring;

#define META_MALLOC(v,n,t) \
          (v = (t*)malloc(((n)*sizeof(t))))
#define META_REALLOC(v,n,t) \
	                  (v = (t*)realloc((v),((n)*sizeof(t))))
#define META_FREE(x) free((x))

static metastring * NewMetaString(const char *init_str)
{
    metastring *s;
    char empty_string[] = "";

    META_MALLOC(s, 1, metastring);
    assert( s != NULL );

    if(init_str == NULL)
	    init_str = empty_string;
    s->length  = strlen(init_str);
    s->bufsize = s->length + 7;  // Prealocate more for potential growth.

    META_MALLOC(s->str, s->bufsize, char);
    assert( s->str != NULL );

    strncpy(s->str, init_str, s->length + 1);
    s->free_string_on_destroy = 1;

    return s;
}


static void DestroyMetaString(metastring *s)
{
    if(s == NULL)
	    return;
    if(s->free_string_on_destroy && (s->str != NULL))
	    META_FREE(s->str);
    META_FREE(s);
}


static void IncreaseBuffer(metastring *s, int chars_needed)
{
    META_REALLOC(s->str, (s->bufsize + chars_needed + 10), char);
    assert( s->str != NULL );
    s->bufsize = s->bufsize + chars_needed + 10;
}


static void MakeUpper(metastring *s)
{
    char *i;
    for(i = s->str; *i; i++)
	    *i = toupper(*i);
}


static int IsVowel(metastring *s, int pos)
{
    char c;
    if((pos < 0) || (pos >= s->length))
	    return 0;
    c = *(s->str + pos);
    if ((c == 'A') || (c == 'E') || (c == 'I') || (c =='O') ||
            (c =='U')  || (c == 'Y'))
	    return 1;
    return 0;
}


static int SlavoGermanic(metastring * s)
{
    if((char *) strstr(s->str, "W"))
	    return 1;
    else if((char *) strstr(s->str, "K"))
	    return 1;
    else if((char *) strstr(s->str, "CZ"))
	    return 1;
    else if((char *) strstr(s->str, "WITZ"))
	    return 1;
    else
	    return 0;
}

static char GetAt(metastring *s, int pos)
{
    if((pos < 0) || (pos >= s->length))
	    return '\0';
    return ((char) *(s->str + pos));
}


static void SetAt(metastring * s, int pos, char c)
{
    if((pos < 0) || (pos >= s->length))
	    return;
    *(s->str + pos) = c;
}

// Caveats: the START value is 0 based.
static int StringAt(metastring * s, int start, int length, ...)
{
    char *test;
    char *pos;
    va_list ap;

    if((start < 0) || (start >= s->length))
        return 0;

    pos = (s->str + start);
    va_start(ap, length);

    do {
	    test = va_arg(ap, char *);
	    if(*test && (strncmp(pos, test, length) == 0))
	        return 1;
    } while(strcmp(test, ""));

    va_end(ap);

    return 0;
}


static void MetaphAdd(metastring * s, char *new_str)
{
    int add_length;

    if (new_str == NULL)
	    return;

    add_length = strlen(new_str);
    if ((s->length + add_length) > (s->bufsize - 1))
	    IncreaseBuffer(s, add_length);

    strcat(s->str, new_str);
    s->length += add_length;
}


static void DoubleMetaphone(char *str, char **codes)
{
    int length;
    metastring *original;
    metastring *primary;
    metastring *secondary;
    int current;
    int last;

    current = 0;
    // Real length and last prior to padding.
    length = strlen(str);
    last = length - 1;
    original = NewMetaString(str);
    // Pad original so we can index beyond end.
    MetaphAdd(original, (char *) "     ");

    primary = NewMetaString((char *) "");
    secondary = NewMetaString((char *) "");

    primary->free_string_on_destroy = 0;
    secondary->free_string_on_destroy = 0;

    MakeUpper(original);

    // Skip these when at start of word.
    if (StringAt(original, 0, 2, "GN", "KN", "PN", "WR", "PS", ""))
	    current += 1;

    // Initial 'X' is pronounced 'Z' e.g. 'Xavier'.
    if (GetAt(original, 0) == 'X') {
	    MetaphAdd(primary, (char *) "S");  // 'Z' maps to 'S'
	    MetaphAdd(secondary, (char *) "S");
	    current += 1;
    }

    while ((primary->length < 4) || (secondary->length < 4)) {
	    if (current >= length)
	        break;
	    switch (GetAt(original, current)) {
	        case 'A':
	        case 'E':
	        case 'I':
	        case 'O':
	        case 'U':
	        case 'Y':  // All init vowels now map to 'A'.
		        if(current == 0) {
		            MetaphAdd(primary, (char *) "A");
		            MetaphAdd(secondary, (char *) "A");
                }
		        current += 1;
		        break;
	        case 'B':  // "-mb", e.g", "dumb", already skipped over...
		        MetaphAdd(primary, (char *) "P");
		        MetaphAdd(secondary, (char *) "P");
                if(GetAt(original, current + 1) == 'B')
		            current += 2;
		        else
		            current += 1;
		        break;
	        case 'Ç':
		        MetaphAdd(primary, (char *) "S");
		        MetaphAdd(secondary, (char *) "S");
		        current += 1;
		        break;
	        case 'C':  // Various germanic.
		        if((current > 1)
                        && !IsVowel(original, current - 2)
                        && StringAt(original, (current - 1), 3, "ACH", "")
                        && ((GetAt(original, current + 2) != 'I')
                            && ((GetAt(original, current + 2) != 'E')
                                || StringAt(original, (current - 2), 6,
                                    "BACHER", "MACHER", ""))))
                {
		            MetaphAdd(primary, (char *) "K");
		            MetaphAdd(secondary, (char *) "K");
		            current += 2;
		            break;
		        }
		        // Special case 'caesar'.
		        if((current == 0)
                        && StringAt(original, current, 6, "CAESAR", ""))
                {
		            MetaphAdd(primary, (char *) "S");
		            MetaphAdd(secondary, (char *) "S");
		            current += 2;
		            break;
		        }
		        // Italian 'chianti'.
		        if(StringAt(original, current, 4, "CHIA", "")) {
		            MetaphAdd(primary, (char *) "K");
		            MetaphAdd(secondary, (char *) "K");
		            current += 2;
		            break;
		        }
		        if(StringAt(original, current, 2, "CH", ""))
                {
                    // Find 'michael'
		            if((current > 0)
                            && StringAt(original, current, 4, "CHAE", ""))
			        {
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "X");
			            current += 2;
			            break;
			        }
		            // Greek roots e.g. 'chemistry', 'chorus'.
		            if((current == 0)
                            && (StringAt(original, (current + 1), 5, "HARAC",
                                    "HARIS", "")
                                || StringAt(original, (current + 1), 3, "HOR",
                                    "HYM", "HIA", "HEM", ""))
                            && !StringAt(original, 0, 5, "CHORE", ""))
			        {
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "K");
			            current += 2;
			            break;
			        }
		            // Germanic, greek, or otherwise 'ch' for 'kh' sound.
		            if((StringAt(original, 0, 4, "VAN ", "VON ", "")
                                || StringAt(original, 0, 3, "SCH", ""))
                            || StringAt(original, (current - 2), 6, "ORCHES",
                                "ARCHIT", "ORCHID", "")
                            || StringAt(original, (current + 2), 1, "T",
                                "S", "")
                            || ((StringAt(original, (current - 1), 1, "A",
                                        "O", "U", "E", "")
                                    || (current == 0))
                                && StringAt(original, (current + 2), 1, "L",
                                    "R", "N", "M", "B", "H", "F", "V", "W",
                                    " ", "")))
			        {
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "K");
			        }
		            else {
			            if(current > 0) {
				            if (StringAt(original, 0, 2, "MC", "")) {
					            // E.g., "McHugh".
					            MetaphAdd(primary, (char *) "K");
					            MetaphAdd(secondary, (char *) "K");
				            }
				            else {
					            MetaphAdd(primary, (char *) "X");
					            MetaphAdd(secondary, (char *) "K");
				            }
			            }
			            else {
				            MetaphAdd(primary, (char *) "X");
				            MetaphAdd(secondary, (char *) "X");
			            }
			        }
		            current += 2;
		            break;
		        }
		        // E.g, 'czerny'.
		        if(StringAt(original, current, 2, "CZ", "")
                        && !StringAt(original, (current - 2), 4, "WICZ", ""))
		        {
		            MetaphAdd(primary, (char *) "S");
		            MetaphAdd(secondary, (char *) "X");
		            current += 2;
		            break;
		        }
		        // E.g., 'focaccia'.
		        if(StringAt(original, (current + 1), 3, "CIA", "")) {
		            MetaphAdd(primary, (char *) "X");
		            MetaphAdd(secondary, (char *) "X");
		            current += 3;
		            break;
		        }
                // Double 'C', but not if e.g. 'McClellan'.
		        if(StringAt(original, current, 2, "CC", "")
                        && !((current == 1) && (GetAt(original, 0) == 'M')))
                {
		            // 'bellocchio' but not 'bacchus'.
		            if (StringAt(original, (current + 2), 1, "I", "E", "H", "")
                            && !StringAt(original, (current + 2), 2, "HU", ""))
		            {
			            // 'accident', 'accede' 'succeed'.
			            if (((current == 1)
                                    && (GetAt(original, current - 1) == 'A'))
                                || StringAt(original, (current - 1), 5, "UCCEE",
                                    "UCCES", ""))
			            {
				            MetaphAdd(primary, (char *) "KS");
				            MetaphAdd(secondary, (char *) "KS");
			            }
			            else {
				            ///* 'bacci', 'bertucci', other italian */
				            MetaphAdd(primary, (char *) "X");
				            MetaphAdd(secondary, (char *) "X");
			            }
			            current += 3;
			            break;
		            }
		            else {  // Pierce's rule.
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "K");
			            current += 2;
			            break;
		            }
                }
		        if(StringAt(original, current, 2, "CK", "CG", "CQ", "")) {
		            MetaphAdd(primary, (char *) "K");
		            MetaphAdd(secondary, (char *) "K");
		            current += 2;
		            break;
		        }
		        if(StringAt(original, current, 2, "CI", "CE", "CY", "")) {
		            // Italian vs english.
		            if(StringAt(original, current, 3, "CIO",
                                "CIE", "CIA", ""))
                    {
			            MetaphAdd(primary, (char *) "S");
			            MetaphAdd(secondary, (char *) "X");
			        }
		            else {
			            MetaphAdd(primary, (char *) "S");
			            MetaphAdd(secondary, (char *) "S");
			        }
		            current += 2;
		            break;
		        }
                // else:
		        MetaphAdd(primary, (char *) "K");
		        MetaphAdd(secondary, (char *) "K");
                // Name sent in 'mac caffrey', 'mac gregor'.
		        if(StringAt(original, (current + 1), 2, " C", " Q", " G", ""))
		            current += 3;
		        else {
		            if (StringAt(original, (current + 1), 1, "C", "K", "Q", "")
                            && !StringAt(original, (current + 1), 2, "CE",
                                "CI", ""))
                        current += 2;
                    else
                        current += 1;
                }
                break;
            case 'D':
		        if(StringAt(original, current, 2, "DG", "")) {
		            if(StringAt(original, (current + 2), 1, "I",
                                "E", "Y", ""))
                    {
			            // E.g. 'edge'.
			            MetaphAdd(primary, (char *) "J");
			            MetaphAdd(secondary, (char *) "J");
			            current += 3;
			            break;
		            }
		            else {
			            // E.g. 'edgar'.
			            MetaphAdd(primary, (char *) "TK");
			            MetaphAdd(secondary, (char *) "TK");
			            current += 2;
			            break;
		            }
                }
                if(StringAt(original, current, 2, "DT", "DD", "")) {
		            MetaphAdd(primary, (char *) "T");
		            MetaphAdd(secondary, (char *) "T");
		            current += 2;
		            break;
		        }
		        // else:
		        MetaphAdd(primary, (char *) "T");
		        MetaphAdd(secondary, (char *) "T");
		        current += 1;
		        break;
	        case 'F':
		        if(GetAt(original, current + 1) == 'F')
		            current += 2;
		        else
		            current += 1;
		        MetaphAdd(primary, (char *) "F");
		        MetaphAdd(secondary, (char *) "F");
		        break;
	        case 'G':
		        if(GetAt(original, current + 1) == 'H') {
		            if((current > 0) && !IsVowel(original, current - 1)) {
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "K");
			            current += 2;
			            break;
			        }
                    if(current < 3) {
			            // 'ghislane', 'ghiradelli'.
			            if(current == 0) {
				            if(GetAt(original, current + 2) == 'I') {
					            MetaphAdd(primary, (char *) "J");
					            MetaphAdd(secondary, (char *) "J");
                            }
				            else {
                                MetaphAdd(primary, (char *) "K");
                                MetaphAdd(secondary, (char *) "K");
				            }
				            current += 2;
				            break;
			            }
			        }
		            // Parker's rule (some further refinements), e.g. 'hugh'.
		            if(((current > 1) && StringAt(original, (current - 2), 1,
                                    "B", "H", "D", ""))
                            || ((current > 2)  // E.g. 'bough'.
                                && StringAt(original, (current - 3), 1, "B",
                                    "H", "D", ""))
                            || ((current > 3)  // E.g. 'boughton'.
			                    && StringAt(original, (current - 4), 1, "B",
                                    "H", "")))
			        {
			            current += 2;
			            break;
			        }
		            else {
			            // E.g., 'laugh', 'McLaughlin', 'cough',
                        // 'gough', 'rough', 'tough'.
			            if((current > 2)
                                && (GetAt(original, current - 1) == 'U')
                                && StringAt(original, (current - 3), 1, "C",
                                    "G", "L", "R", "T", ""))
			            {
				            MetaphAdd(primary, (char *) "F");
				            MetaphAdd(secondary, (char *) "F");
			            }
			            else if((current > 0)
                                && GetAt(original, current - 1) != 'I')
			            {
                            MetaphAdd(primary, (char *) "K");
				            MetaphAdd(secondary, (char *) "K");
			            }
			            current += 2;
			            break;
                    }
                }
                if(GetAt(original, current + 1) == 'N') {
                    if((current == 1) && IsVowel(original, 0)
                            && !SlavoGermanic(original))
                    {
			            MetaphAdd(primary, (char *) "KN");
			            MetaphAdd(secondary, (char *) "N");
			        }
		            else {
			            // Not e.g. 'cagney'.
			            if(!StringAt(original, (current + 2), 2, "EY", "")
                                && (GetAt(original, current + 1) != 'Y')
                                && !SlavoGermanic(original))
			            {
			                MetaphAdd(primary, (char *) "N");
			                MetaphAdd(secondary, (char *) "KN");
			            }
		                else {
			                MetaphAdd(primary, (char *) "KN");
		                    MetaphAdd(secondary, (char *) "KN");
                        }
		                current += 2;
		                break;
                    }
		        }
		        // 'tagliaro'.
		        if (StringAt(original, (current + 1), 2, "LI", "")
                        && !SlavoGermanic(original))
		        {
		            MetaphAdd(primary, (char *) "KL");
		            MetaphAdd(secondary, (char *) "L");
		            current += 2;
		            break;
		        }
		        // -ges-, -gep-, -gel-, -gie- at beginning.
		        if((current == 0)
                        && ((GetAt(original, current + 1) == 'Y')
                            || StringAt(original, (current + 1), 2, "ES",
                                "EP", "EB", "EL", "EY", "IB", "IL", "IN",
                                "IE", "EI", "ER", "")))
		        {
		            MetaphAdd(primary, (char *) "K");
		            MetaphAdd(secondary, (char *) "J");
		            current += 2;
		            break;
		        }
		        // -ger-,  -gy-.
		        if((StringAt(original, (current + 1), 2, "ER", "")
                            || (GetAt(original, current + 1) == 'Y'))
                        && !StringAt(original, 0, 6, "DANGER",
                            "RANGER", "MANGER", "")
                        && !StringAt(original, (current - 1), 1, "E", "I", "")
                        && !StringAt(original, (current - 1), 3, "RGY",
                            "OGY", ""))
                {
		            MetaphAdd(primary, (char *) "K");
		            MetaphAdd(secondary, (char *) "J");
		            current += 2;
		            break;
		        }
                // Italian e.g, 'biaggi'.
		        if(StringAt(original, (current + 1), 1, "E", "I", "Y", "")
                        || StringAt(original, (current - 1), 4, "AGGI",
                            "OGGI", ""))
                {
		            // Obvious germanic.
		            if((StringAt(original, 0, 4, "VAN ", "VON ", "")
                                || StringAt(original, 0, 3, "SCH", ""))
                            || StringAt(original, (current + 1), 2, "ET", ""))
			        {
			            MetaphAdd(primary, (char *) "K");
			            MetaphAdd(secondary, (char *) "K");
			        }
		            else {
			            // Always soft if french ending.
			            if(StringAt(original, (current + 1), 4, "IER ", "")) {
				            MetaphAdd(primary, (char *) "J");
				            MetaphAdd(secondary, (char *) "J");
			            }
			            else {
				            MetaphAdd(primary, (char *) "J");
				            MetaphAdd(secondary, (char *) "K");
			            }
			        }
		            current += 2;
		            break;
		        }
                if(GetAt(original, current + 1) == 'G')
                    current += 2;
		        else
		            current += 1;
		        MetaphAdd(primary, (char *) "K");
		        MetaphAdd(secondary, (char *) "K");
		        break;
            case 'H':
                // Only keep if first & before vowel or btw. 2 vowels.
		        if(((current == 0) || IsVowel(original, current - 1))
                        && IsVowel(original, current + 1))
                {
                    MetaphAdd(primary, (char *) "H");
                    MetaphAdd(secondary, (char *) "H");
                    current += 2;
		        }
		        else  // Also takes care of 'HH'.
		            current += 1;
		        break;
	        case 'J':
                // Obvious spanish, 'jose', 'san jacinto'.
		        if(StringAt(original, current, 4, "JOSE", "")
                        || StringAt(original, 0, 4, "SAN ", ""))
                {
                    if(((current == 0)
                                && (GetAt(original, current + 4) == ' '))
                            || StringAt(original, 0, 4, "SAN ", ""))
                    {
			            MetaphAdd(primary, (char *) "H");
			            MetaphAdd(secondary, (char *) "H");
			        }
                    else {
			            MetaphAdd(primary, (char *) "J");
			            MetaphAdd(secondary, (char *) "H");
			        }
		            current += 1;
		            break;
		        }
		        if((current == 0)
                        && !StringAt(original, current, 4, "JOSE", ""))
                {
                    // Yankelovich / Jankelowicz
		            MetaphAdd(primary, (char *) "J");
		            MetaphAdd(secondary, (char *) "A");
		        }
		        else {
                    // Spanish pron. of e.g. 'bajador'.
		            if(IsVowel(original, current - 1)
                            && !SlavoGermanic(original)
                            && ((GetAt(original, current + 1) == 'A')
                                || (GetAt(original, current + 1) == 'O')))
			        {
			            MetaphAdd(primary, (char *) "J");
			            MetaphAdd(secondary, (char *) "H");
			        }
		            else {
			            if(current == last) {
				            MetaphAdd(primary, (char *) "J");
				            MetaphAdd(secondary, (char *) "");
			            }
			            else {
				            if(!StringAt(original, (current + 1), 1, "L", "T",
                                        "K", "S", "N", "M", "B", "Z", "")
                                    && !StringAt(original, (current - 1), 1,
                                        "S", "K", "L", ""))
                            {
				                MetaphAdd(primary, (char *) "J");
				                MetaphAdd(secondary, (char *) "J");
                            }
			            }
			        }
		        }
		        if(GetAt(original, current + 1) == 'J')
		            current += 2;
		        else
		            current += 1;
		        break;
	        case 'K':
		        if(GetAt(original, current + 1) == 'K')
		            current += 2;
		        else
		            current += 1;
		        MetaphAdd(primary, (char *) "K");
		        MetaphAdd(secondary, (char *) "K");
		        break;
	        case 'L':
                if(GetAt(original, current + 1) == 'L') {
		            // Spanish e.g. 'cabrillo', 'gallegos'.
		            if(((current == (length - 3))
                                && StringAt(original, (current - 1), 4,
                                    "ILLO", "ILLA", "ALLE", ""))
                            || ((StringAt(original, (last - 1), 2,
                                        "AS", "OS", "")
                                    || StringAt(original, last, 1,
                                        "A", "O", ""))
                                && StringAt(original, (current - 1), 4,
                                    "ALLE", "")))
			        {
			            MetaphAdd(primary, (char *) "L");
			            MetaphAdd(secondary, (char *) "");
			            current += 2;
			            break;
			        }
		            current += 2;
		        }
		        else
		            current += 1;
		        MetaphAdd(primary, (char *) "L");
		        MetaphAdd(secondary, (char *) "L");
		        break;
	        case 'M':
		        if((StringAt(original, (current - 1), 3, "UMB", "")
                            && (((current + 1) == last)
                                || StringAt(original, (current + 2), 2,
                                    "ER", "")))
                        || (GetAt(original, current + 1) == 'M'))
		            current += 2;
		        else
		            current += 1;
		        MetaphAdd(primary, (char *) "M");
		        MetaphAdd(secondary, (char *) "M");
		        break;
	        case 'N':
		        if(GetAt(original, current + 1) == 'N')
                    current += 2;
		        else
                    current += 1;
		        MetaphAdd(primary, (char *) "N");
		        MetaphAdd(secondary, (char *) "N");
		        break;
	        case 'Ñ':
		        current += 1;
		        MetaphAdd(primary, (char *) "N");
		        MetaphAdd(secondary, (char *) "N");
		        break;
	        case 'P':
		        if(GetAt(original, current + 1) == 'H') {
		            MetaphAdd(primary, (char *) "F");
		            MetaphAdd(secondary, (char *) "F");
		            current += 2;
		            break;
		        }
                // Also account for 'campbell', 'raspberry'.
	    	    if(StringAt(original, (current + 1), 1, "P", "B", ""))
                    current += 2;
                else
                    current += 1;
                MetaphAdd(primary, (char *) "P");
                MetaphAdd(secondary, (char *) "P");
	    	    break;
	        case 'Q':
                if(GetAt(original, current + 1) == 'Q')
                    current += 2;
	    	    else
                    current += 1;
	    	    MetaphAdd(primary, (char *) "K");
	    	    MetaphAdd(secondary, (char *) "K");
	    	    break;
	        case 'R':
	    	    // French e.g. 'rogier', but exclude 'hochmeier'.
	    	    if((current == last)
                        && !SlavoGermanic(original)
                        && StringAt(original, (current - 2), 2, "IE", "")
                        && !StringAt(original, (current - 4), 2, "ME",
                            "MA", ""))
	    	    {
	    	        MetaphAdd(primary, (char *) "");
	    	        MetaphAdd(secondary, (char *) "R");
                }
	    	    else {
	    	        MetaphAdd(primary, (char *) "R");
	    	        MetaphAdd(secondary, (char *) "R");
	    	    }
	    	    if(GetAt(original, current + 1) == 'R')
                    current += 2;
	    	    else
                    current += 1;
	    	    break;
	        case 'S':
	    	    // Special cases 'island', 'isle', 'carlisle', 'carlysle'.
	    	    if(StringAt(original, (current - 1), 3, "ISL", "YSL", "")) {
	    	        current += 1;
	    	        break;
	    	    }
                // Special case 'sugar-'.
	    	    if((current == 0)
                        && StringAt(original, current, 5, "SUGAR", ""))
                {
	    	        MetaphAdd(primary, (char *) "X");
	    	        MetaphAdd(secondary, (char *) "S");
	    	        current += 1;
	    	        break;
	    	    }
	    	    if(StringAt(original, current, 2, "SH", "")) {
	    	        // Germanic.
	    	        if(StringAt(original, (current + 1), 4,
                                "HEIM", "HOEK", "HOLM", "HOLZ", ""))
                    {
	    		        MetaphAdd(primary, (char *) "S");
	    		        MetaphAdd(secondary, (char *) "S");
	    		    }
	    	        else {
	    		        MetaphAdd(primary, (char *) "X");
	    		        MetaphAdd(secondary, (char *) "X");
	    		    }
	    	        current += 2;
	    	        break;
	    	    }
	    	    // Italian & armenian.
	    	    if(StringAt(original, current, 3, "SIO", "SIA", "")
                        || StringAt(original, current, 4, "SIAN", ""))
                {
                    if(!SlavoGermanic(original)) {
	    		        MetaphAdd(primary, (char *) "S");
	    		        MetaphAdd(secondary, (char *) "X");
                    }
                    else {
	    		        MetaphAdd(primary, (char *) "S");
	    		        MetaphAdd(secondary, (char *) "S");
                    }
                    current += 3;
                    break;
	    	    }
	    	    // German & anglicisations, e.g. 'smith' match 'schmidt',
                // 'snider' match 'schneider'. Also, -sz- in slavic language
                // altho in hungarian it is pronounced 's'.
	    	    if(((current == 0)
                            && StringAt(original, (current + 1), 1, "M",
                                "N", "L", "W", ""))
                        || StringAt(original, (current + 1), 1, "Z", ""))
                {
	    	        MetaphAdd(primary, (char *) "S");
	    	        MetaphAdd(secondary, (char *) "X");
	    	        if(StringAt(original, (current + 1), 1, "Z", ""))
	    		        current += 2;
	    	        else
	    		        current += 1;
	    	        break;
                }
                if(StringAt(original, current, 2, "SC", "")) {
	    	        // Schlesinger's rule.
	    	        if(GetAt(original, current + 2) == 'H') {
	    		        // Dutch origin, e.g. 'school', 'schooner'.
	    		        if(StringAt(original, (current + 3), 2, "OO",
                                    "ER", "EN", "UY", "ED", "EM", ""))
                        {
	    			        // 'schermerhorn', 'schenker'.
	    			        if(StringAt(original, (current + 3), 2, "ER",
                                        "EN", ""))
                            {
	    			            MetaphAdd(primary, (char *) "X");
	    			            MetaphAdd(secondary, (char *) "SK");
	    			        }
	    			        else {
	    			            MetaphAdd(primary, (char *) "SK");
	    			            MetaphAdd(secondary, (char *) "SK");
                            }
	    			        current += 3;
	    			        break;
	    		        }
	    		        else {
	    			        if((current == 0) && !IsVowel(original, 3)
                                    && (GetAt(original, 3) != 'W'))
                            {
	    			            MetaphAdd(primary, (char *) "X");
	    			            MetaphAdd(secondary, (char *) "S");
	    			        }
	    			        else {
	    			            MetaphAdd(primary, (char *) "X");
	    			            MetaphAdd(secondary, (char *) "X");
	    			        }
	    			        current += 3;
	    			        break;
	    		        }
                    }
	    	        if(StringAt(original, (current + 2), 1, "I",
                                "E", "Y", ""))
                    {
	    		        MetaphAdd(primary, (char *) "S");
	    		        MetaphAdd(secondary, (char *) "S");
	    		        current += 3;
	    		        break;
	    		    }
	    	        // else:
	    	        MetaphAdd(primary, (char *) "SK");
	    	        MetaphAdd(secondary, (char *) "SK");
	    	        current += 3;
	    	        break;
	    	    }
	    	    // French e.g. 'resnais', 'artois'.
                if((current == last)
                        && StringAt(original, (current - 2), 2, "AI", "OI", ""))
	    	    {
	    	        MetaphAdd(primary, (char *) "");
	    	        MetaphAdd(secondary, (char *) "S");
	    	    }
	    	    else {
	    	        MetaphAdd(primary, (char *) "S");
	    	        MetaphAdd(secondary, (char *) "S");
	    	    }
	    	    if(StringAt(original, (current + 1), 1, "S", "Z", ""))
	    	        current += 2;
	    	    else
	    	        current += 1;
	    	    break;
	        case 'T':
	    	    if(StringAt(original, current, 4, "TION", "")) {
	    	        MetaphAdd(primary, (char *) "X");
	    	        MetaphAdd(secondary, (char *) "X");
	    	        current += 3;
	    	        break;
	    	    }
	    	    if(StringAt(original, current, 3, "TIA", "TCH", "")) {
	    	        MetaphAdd(primary, (char *) "X");
	    	        MetaphAdd(secondary, (char *) "X");
	    	        current += 3;
	    	        break;
	    	    }
	    	    if(StringAt(original, current, 2, "TH", "")
                        || StringAt(original, current, 3, "TTH", ""))
                {
	    	        // Special case 'thomas', 'thames' or germanic.
	    	        if(StringAt(original, (current + 2), 2, "OM", "AM", "")
                            || StringAt(original, 0, 4, "VAN ", "VON ", "")
                            || StringAt(original, 0, 3, "SCH", ""))
	    		    {
	    		        MetaphAdd(primary, (char *) "T");
	    		        MetaphAdd(secondary, (char *) "T");
	    		    }
	    	        else {
	    		        MetaphAdd(primary, (char *) "0"); /* yes, zero */
	    		        MetaphAdd(secondary, (char *) "T");
	    		    }
	    	        current += 2;
	    	        break;
	    	    }
	    	    if(StringAt(original, (current + 1), 1, "T", "D", ""))
	    	        current += 2;
	    	    else
	    	        current += 1;
	    	    MetaphAdd(primary, (char *) "T");
	    	    MetaphAdd(secondary, (char *) "T");
	    	    break;
	        case 'V':
	    	    if(GetAt(original, current + 1) == 'V')
	    	        current += 2;
	    	    else
	    	        current += 1;
	    	    MetaphAdd(primary, (char *) "F");
	    	    MetaphAdd(secondary, (char *) "F");
	    	    break;
	        case 'W':
	    	    // Can also be in middle of word.
	    	    if(StringAt(original, current, 2, "WR", "")) {
	    	        MetaphAdd(primary, (char *) "R");
	    	        MetaphAdd(secondary, (char *) "R");
	    	        current += 2;
	    	        break;
	    	    }
	    	    if((current == 0)
                        && (IsVowel(original, current + 1)
                            || StringAt(original, current, 2, "WH", "")))
	    	    {
	    	        // Wasserman should match Vasserman.
	    	        if(IsVowel(original, current + 1)) {
	    		        MetaphAdd(primary, (char *) "A");
	    		        MetaphAdd(secondary, (char *) "F");
	    		    }
	    	        else {
	    		    // need Uomo to match Womo.
	    		        MetaphAdd(primary, (char *) "A");
	    		        MetaphAdd(secondary, (char *) "A");
	    		    }
	    	    }
	    	    // Arnow should match Arnoff.
	    	    if(((current == last) && IsVowel(original, current - 1))
                        || StringAt(original, (current - 1), 5, "EWSKI",
                            "EWSKY", "OWSKI", "OWSKY", "")
                        || StringAt(original, 0, 3, "SCH", ""))
	    	    {
	    	        MetaphAdd(primary, (char *) "");
	    	        MetaphAdd(secondary, (char *) "F");
	    	        current += 1;
	    	        break;
	    	    }
	    	    // Polish e.g. 'filipowicz'.
	    	    if(StringAt(original, current, 4, "WICZ", "WITZ", ""))
                {
	    	        MetaphAdd(primary, (char *) "TS");
	    	        MetaphAdd(secondary, (char *) "FX");
	    	        current += 4;
	    	        break;
	    	    }
	    	    // else: skip
	    	    current += 1;
	    	    break;
	        case 'X':
	    	    // french e.g. breaux.
	    	    if(!((current == last)
                            && (StringAt(original, (current - 3), 3,
                                    "IAU", "EAU", "")
                                || StringAt(original, (current - 2), 2,
                                    "AU", "OU", ""))))
                {
	    	        MetaphAdd(primary, (char *) "KS");
	    	        MetaphAdd(secondary, (char *) "KS");
                }
                if(StringAt(original, (current + 1), 1, "C", "X", ""))
	    	        current += 2;
	    	    else
	    	        current += 1;
	    	    break;
	        case 'Z':
	    	    // Chinese pinyin e.g. 'zhao'.
	    	    if(GetAt(original, current + 1) == 'H') {
	    	        MetaphAdd(primary, (char *) "J");
	    	        MetaphAdd(secondary, (char *) "J");
	    	        current += 2;
	    	        break;
	    	    }
	    	    else if (StringAt(original, (current + 1), 2,
                            "ZO", "ZI", "ZA", "")
                        || (SlavoGermanic(original) && ((current > 0)
                                && GetAt(original, current - 1) != 'T')))
                {
	    	        MetaphAdd(primary, (char *) "S");
	    	        MetaphAdd(secondary, (char *) "TS");
	    	    }
	    	    else {
	    	        MetaphAdd(primary, (char *) "S");
	    	        MetaphAdd(secondary, (char *) "S");
                }
                if(GetAt(original, current + 1) == 'Z')
	    	        current += 2;
	    	    else
	    	        current += 1;
	    	    break;
	        default:
	    	    current += 1;
        }
    }
    if(primary->length > 4)
	    SetAt(primary, 4, '\0');

    if(secondary->length > 4)
	    SetAt(secondary, 4, '\0');

    *codes = primary->str;
    *++codes = secondary->str;

    DestroyMetaString(original);
    DestroyMetaString(primary);
    DestroyMetaString(secondary);
}

/** C++ wrapper. */

typedef std::pair<std::string, std::string> ss;

ss double_metaphone(const std::string str)
{
    size_t size = str.size();
    char* cstr = (char *) malloc((size + 1) * sizeof(char));
    strncpy(cstr, str.c_str(), size);
    cstr[size] = '\0';
    char* codes[2];
    DoubleMetaphone(cstr, codes);
    ss codes_pair = make_pair(std::string(codes[0]), std::string(codes[1]));
    free(codes[0]);
    free(codes[1]);
    free(cstr);
    return codes_pair;
}
