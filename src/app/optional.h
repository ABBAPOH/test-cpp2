#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <assert.h>
#include <utility>

template<typename T> class Optional;
typedef Optional<void> Nothing;

template<typename T>
class Optional
{
private:
    typedef bool (Optional::*RestrictedBool)() const;
public:

    inline Optional() : m_isNull(true), m_value() {}
    inline Optional(const Nothing &) : m_isNull(true), m_value() {}
    inline Optional(const T &value) : m_isNull(false), m_value(value) {}
    template <typename U>
    inline Optional(const U &value) : m_isNull(false), m_value(value) {}
    inline Optional(T &&value) : m_isNull(false), m_value(std::move(value)) {}

    template <typename U>
    inline Optional(const Optional<U> &other) :
        m_isNull(other.isNull()),
        m_value(other.value())
    {}

    inline Optional &operator=(const T &value)
    {
        m_isNull = false;
        m_value = value;
        return *this;
    }

    template <typename U>
    inline Optional &operator=(const Optional<U> &other)
    {
        m_isNull = false;
        m_value = other.value();
        return *this;
    }

    inline Optional &operator=(const Nothing &)
    {
        m_isNull = true;
        m_value = T();
        return *this;
    }

    // compiler-generated copy/move ctors/assignment operators are ok!
    // compiler-generated dtor is ok!

    inline bool isNull() const { return m_isNull; }

    inline T &value()
    {
        assert(!m_isNull);
        return m_value;
    }

    inline const T &value() const
    {
        assert(!m_isNull);
        return m_value;
    }

    template <typename U>
    inline T value(const U &defaultValue) const
    {
        return m_isNull ? defaultValue : m_value;
    }

    inline void swap(Optional &other)
    {
        qSwap(m_isNull, other.m_isNull);
        qSwap(m_value, other.m_value);
    }

    inline void reset() { m_isNull = true; m_value = T(); }

    inline T &operator*()
    {
        assert(!m_isNull);
        return m_value;
    }

    inline const T &operator*() const
    {
        assert(!m_isNull);
        return m_value;
    }

    inline T *operator->()
    {
        assert(!m_isNull);
        return &m_value;
    }

    inline const T *operator->() const
    {
        assert(!m_isNull);
        return &m_value;
    }

    inline operator RestrictedBool() const { return m_isNull ? nullptr : &Optional::isNull; }

private:
    bool m_isNull;
    T m_value;
};

template <>
class Optional<void>
{
private:
    typedef bool (Optional<void>::*RestrictedBool)() const;
public:
    inline bool isNull() const { return true; }

    inline operator RestrictedBool () const { return nullptr; }
};

template<typename T>
inline bool operator==(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return (rhs.isNull() && lhs.isNull())
            || (!rhs.isNull() && !lhs.isNull() && lhs.value() == rhs.value());
}

template<typename T>
inline bool operator==(const Optional<T> &lhs, const T &rhs)
{
    return !lhs.isNull() && (lhs.value() == rhs);
}

template<typename T>
inline bool operator==(const T &value, const Optional<T> &optional)
{
    return !optional.isNull() && (optional.value() == value);
}

template<typename T>
inline bool operator!=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline bool operator!=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline bool operator!=(const T &lhs, const Optional<T> &rhs)
{
    return !(rhs == lhs);
}

template<typename T>
inline bool operator<(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return (lhs.isNull() && !rhs.isNull())
            || (!rhs.isNull() && !lhs.isNull() && lhs.value() < rhs.value());
}

template<typename T>
inline bool operator<(const Optional<T> &lhs, const T &rhs)
{
    return lhs.isNull() || (lhs.value() < rhs);
}

template<typename T>
inline bool operator<(const T &lhs, const Optional<T> &rhs)
{
    return !rhs.isNull() && (lhs < rhs.value());
}

template<typename T>
inline bool operator>(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const Optional<T> &lhs, const T &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const T &lhs, const Optional<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator<=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator<=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator<=(const T &lhs, const Optional<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator>=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline bool operator>=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline bool operator>=(const T &lhs, const Optional<T> &rhs)
{
    return !(lhs < rhs);
}

#endif // OPTIONAL_H
