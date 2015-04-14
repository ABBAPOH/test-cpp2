#ifndef RESULT_H
#define RESULT_H

#include "optional.h"
#include <string>

class Error;

template<typename T>
class Result
{
private:
    typedef bool (Result::*RestrictedBool)() const;

public:
    inline Result() :
        m_hasError(false),
        m_errorString(std::string()),
        m_value(T())
    {}

    inline Result(const T &value) :
        m_hasError(false),
        m_errorString(std::string()),
        m_value(value)
    {}

    template <typename U>
    inline Result(const U &value) :
        m_hasError(false),
        m_errorString(std::string()),
        m_value(value)
    {}

    inline Result(T &&value) :
        m_hasError(false),
        m_errorString(std::string()),
        m_value(std::move(value))
    {}

    template <typename U>
    inline Result(const Result<U> &other) :
        m_hasError(other.hasError()),
        m_errorString(other.errorString()),
        m_value(other.value())
    {}

    inline Result(const Error &error);

    inline Result &operator=(const T &value)
    {
        m_hasError = false;
        m_errorString.clear();
        m_value = value;
        return *this;
    }

    template <typename U>
    inline Result &operator=(const Result<U> &other)
    {
        m_hasError = false;
        m_errorString.clear();
        m_value = other.value();
        return *this;
    }

    inline Result &operator=(const Error &error);

    inline bool isValid() const { return !m_hasError; }
    inline bool hasError() const { return m_hasError; }

    inline std::string errorString() const { return m_errorString; }

    inline T &value()
    {
        Q_ASSERT_X(isValid(), "Result::value", "Result is invalid");
        return m_value;
    }

    inline const T &value() const
    {
        Q_ASSERT_X(isValid(), "Result::value", "Result is invalid");
        return m_value;
    }

    template <typename U>
    inline T value(const U &defaultValue) const
    {
        return !isValid() ? defaultValue : m_value;
    }

    inline Optional<T> toOptional() const
    {
        return isValid() ? Optional<T>(m_value) : Nothing();
    }

    inline void swap(Result &other)
    {
        qSwap(m_hasError, other.m_hasError);
        qSwap(m_errorString, other.m_errorString);
        qSwap(m_value, other.m_value);
    }

    inline void reset()
    {
        m_hasError = false;
        m_errorString.clear();
        m_value = T();
    }

    inline T &operator*()
    {
        Q_ASSERT_X(isValid(), "Result::operator*", "Result is invalid");
        return m_value;
    }

    inline const T &operator*() const
    {
        Q_ASSERT_X(isValid(), "Result::operator*", "Result is invalid");
        return m_value;
    }

    inline T *operator->()
    {
        Q_ASSERT_X(isValid(), "Result::operator->", "Result is invalid");
        return &m_value;
    }

    inline const T *operator->() const
    {
        Q_ASSERT_X(isValid(), "Result::operator->", "Result is invalid");
        return &m_value;
    }

    inline operator RestrictedBool() const { return isValid() ? &Result::isValid : nullptr; }

    inline operator Optional<T>() const { return toOptional(); }

private:
    bool m_hasError;
    std::string m_errorString;
    T m_value;
};

template<>
class Result<void>
{
private:
    typedef bool (Result::*RestrictedBool)() const;

public:
    inline Result() : m_hasError(false), m_errorString(std::string()) {}
    inline Result(const Nothing &) : m_hasError(false), m_errorString(std::string()) {}
    inline Result(const Error &error);

    inline bool isValid() const { return !m_hasError; }
    inline bool hasError() const { return m_hasError; }
    inline std::string errorString() const { return m_errorString; }

    inline void swap(Result<void> &other)
    {
        using std::swap;
        swap(m_hasError, other.m_hasError);
        swap(m_errorString, other.m_errorString);
    }

    inline void reset()
    {
        m_hasError = false;
        m_errorString.clear();
    }

    inline operator RestrictedBool() const { return isValid() ? &Result::isValid : nullptr; }

protected:
    inline Result(const std::string &errorString, bool hasError) :
        m_hasError(hasError),
        m_errorString(errorString)
    {}

private:
    bool m_hasError;
    std::string m_errorString;
};

class Error : public Result<void>
{
public:
    inline explicit Error(const std::string &errorString) :
        Result(errorString, true)
    {}

    template<typename T>
    inline explicit Error(const Result<T> &other) :
        Result(other.errorString(), true)
    {}
};

template<typename T>
inline Result<T>::Result(const Error &error) :
    m_hasError(error.hasError()),
    m_errorString(error.errorString()),
    m_value(T())
{}

Result<void>::Result(const Error &error) :
    m_hasError(error.hasError()),
    m_errorString(error.errorString())
{
}

template<typename T>
Result<T> &Result<T>::operator=(const Error &error)
{
    m_hasError = error.hasError();
    m_errorString = error.errorString();
    m_value = T();
    return *this;
}

template<typename T>
inline bool operator==(const Result<T> &lhs, const Result<T> &rhs)
{
    return (!lhs.isValid() && !rhs.isValid())
            || (lhs.isValid() && rhs.isValid() && lhs.value() == rhs.value());
}

template<typename T>
inline bool operator==(const Result<T> &lhs, const T &rhs)
{
    return lhs.isValid() && (lhs.value() == rhs);
}

template<typename T>
inline bool operator==(const T &lhs, const Result<T> &rhs)
{
    return rhs.isValid() && (rhs.value() == lhs);
}

template<typename T>
inline bool operator!=(const Result<T> &lhs, const Result<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline bool operator!=(const Result<T> &lhs, const T &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline bool operator!=(const T &lhs, const Result<T> &rhs)
{
    return !(rhs == lhs);
}

template<typename T>
inline bool operator<(const Result<T> &lhs, const Result<T> &rhs)
{
    return (!lhs.isValid() && rhs.isValid())
            || (lhs.isValid() && rhs.isValid() && lhs.value() < rhs.value());
}

template<typename T>
inline bool operator<(const Result<T> &lhs, const T &rhs)
{
    return !lhs.isValid() || (lhs.value() < rhs);
}

template<typename T>
inline bool operator<(const T &lhs, const Result<T> &rhs)
{
    return rhs.isValid() && (lhs < rhs.value());
}

template<typename T>
inline bool operator>(const Result<T> &lhs, const Result<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const Result<T> &lhs, const T &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator>(const T &lhs, const Result<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator<=(const Result<T> &lhs, const Result<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator<=(const Result<T> &lhs, const T &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator<=(const T &lhs, const Result<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
inline bool operator>=(const Result<T> &lhs, const Result<T> &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline bool operator>=(const Result<T> &lhs, const T &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline bool operator>=(const T &lhs, const Result<T> &rhs)
{
    return !(lhs < rhs);
}

#endif // RESULT_H
