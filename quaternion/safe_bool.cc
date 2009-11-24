#include "./safe_bool.h"

SafeBoolBase& SafeBoolBase::operator=(const SafeBoolBase& sb)
{
    return *this;
}

template<typename T>
SafeBool<T>::operator bool_type() const
{
    return (static_cast<const T*>(this))->booleanTest() ?
        &SafeBoolBase::noComparisonSupport : 0;
}

SafeBool<void>::operator bool_type() const
{
    return booleanTest() ?
        &SafeBoolBase::noComparisonSupport : 0;
    SafeBoolBase::noComparisonSupport();
}

template<typename T>
Boolean operator==(const SafeBool<T>& lhs, Boolean b)
{
    if(b) {
        if(lhs)
            return true;
        else
            return false;
    }
    else {
        if(lhs)
            return false;
        else
            return true;
    }
}

template<typename T>
Boolean operator==(Boolean b, const SafeBool<T>& rhs)
{
    if(b) {
        if(rhs)
            return true;
        else
            return false;
    }
    else {
        if(rhs)
            return false;
        else
            return true;
    }
}

template<typename T, typename U>
void operator==(const SafeBool<T>& lhs, const SafeBool<U>& rhs)
{
    lhs.noComparisonSupport();
}

template<typename T, typename U>
void operator!=(const SafeBool<T>& lhs, const SafeBool<U>& rhs)
{
    lhs.noComparisonSupport();
}


