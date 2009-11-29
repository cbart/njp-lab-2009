/** Safe Boolean converter base class.                           *
 * author: Cezary Bartoszuk <cbart@students.mimuw.edu.pl>        *
 *     id: cb277617@students.mimuw.edu.pl                        *
 * usage:                                                        *
 *   When you want to make your class safe-if-then-else tested,  *
 *   e.g. you want: if(a) to work and don't want a == b to work  *
 *   (when not defining the == operator explicitly) simply make  *
 *   your class inherit from SafeBool<> and define protected     *
 *   "bool booleanTest() const" method, which will be invoked    *
 *   in if(a)-then-else (with a - object of your class).         */

#ifndef _SAFE_BOOL_H_
#define _SAFE_BOOL_H_

class SafeBoolBase
{

    protected:

        SafeBoolBase() {}
        SafeBoolBase(const SafeBoolBase& sbb) {}
        SafeBoolBase& operator=(const SafeBoolBase& sbb);
        ~SafeBoolBase() {}

    public:

        typedef void (SafeBoolBase::*bool_type)() const;
        void noComparisonSupport() const {};

};

template<typename T=void>
class SafeBool : private SafeBoolBase
{

    protected:

        ~SafeBool() {}

    public:

        operator bool_type() const;

};

template<>
class SafeBool<void> : private SafeBoolBase
{

    protected:

        virtual bool booleanTest() const = 0;
        virtual ~SafeBool() {}

    public:

        operator bool_type() const;

};

template<typename T, typename U>
void operator==(const SafeBool<T>& lhs, const SafeBool<U>& rhs);

template<typename T, typename U>
void operator!=(const SafeBool<T>& lhs, const SafeBool<U>& rhs);

#endif
