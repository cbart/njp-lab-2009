#ifndef _SAFE_BOOL_H_
#define _SAFE_BOOL_H_

typedef bool Boolean;

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

        virtual Boolean booleanTest() const = 0;
        virtual ~SafeBool() {}

    public:

        operator bool_type() const;
        //{
            //return booleanTest() ?
                //&SafeBoolBase::noComparisonSupport : 0;
            //SafeBoolBase::noComparisonSupport();
        //}
};

template<typename T>
Boolean operator==(const SafeBool<T>& lhs, Boolean b);

template<typename T>
Boolean operator==(Boolean b, const SafeBool<T>& rhs);

template<typename T, typename U>
void operator==(const SafeBool<T>& lhs, const SafeBool<U>& rhs);

template<typename T, typename U>
void operator!=(const SafeBool<T>& lhs, const SafeBool<U>& rhs);

#endif
