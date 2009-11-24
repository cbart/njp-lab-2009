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


