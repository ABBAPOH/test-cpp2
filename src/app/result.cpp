#include "result.h"

/*!
    \class Result
*/

/*!
    \fn Result<T>::Result()

    Constructs an Result with default T value and 0 error code.
*/

/*!
    \fn Result<T>::Result(const T &value)

    Constructs an Result object and from the \a value; error code is 0.
*/

/*!
    \fn Result<T>::Result(const Result<U> &other)

    Constructs a new Result object that is a copy of the given \a other
    object.

    This constructor tries to implicitly convert value from U type to T.
*/

/*!
    \fn Result<T>::Result(const Error &error)

    Constructs a new Result object that is a copy of the given \a error
    object.
*/


/*!
    \fn Result<T> &Result<T>::operator=(const T &value)

    Assigns contents to the \a value and makes this object non-empty.
*/

/*!
    \fn bool Result<T>::hasError() const

    Returns true if the error code is not zero.
*/

/*!
    \fn T &Result<T>::value()

    Returns the reference to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn const T &Result<T>::value() const

    Returns the const reference to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn const T &Result<T>::value(const U &defaultValue) const

    Returns the contained value if object is not empty or \a defaultValue
    otherwise.
*/

/*!
    \fn void Result<T>::swap(Result &other);

    Swaps this Result object with the \a other.
*/

/*!
    \fn void Result<T>::reset();

    Makes this Result object empty.
*/

/*!
    \fn T &Result<T>::operator*()

    Returns a reference to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn const T &Result<T>::operator*() const

    Returns a const reference to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn T *Result<T>::operator->()

    Returns a pointer to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn const T *Result<T>::operator->() const

    Returns a const pointer to the contained value.

    \note This function asserts if this object has error.
*/

/*!
    \fn Result<T>::operator bool() const

    Returns true if this Result object is not empty.
*/

/*!
    \fn bool operator==(const Result<T> &left, const Result<T> &right)

    \relates Result

    Returns true if the \a left Result object is equal to the \a right
    object.
*/

/*!
    \fn bool operator<(const Result<T> &left, const Result<T> &right)

    \relates Result

    Returns true if the \a left Result object is less than the \a right
    object.

    \note Empty Result is less than non-empty Result.
*/

/*!
    \fn bool operator>(const Result<T> &left, const Result<T> &right)

    \relates Result

    Returns true if the \a left Result object is greater than the \a right
    object.

    \note Non-empty Result is greater than an empty Result.
*/

/*!
    \fn bool operator==(const Result<T> &Result, const T &value)

    \relates Result

    Returns true if the \a Result contains the \a value.
*/

/*!
    \fn bool operator==(const T &value, const Result<T> &Result)

    \relates Result

    Returns true if the \a Result contains the \a value.
*/

/*!
    \fn bool operator<(const Result<T> &Result, const T &value)

    \relates Result

    Returns true if the \a Result's value is less than the \a value.

    \note Empty Result is always less than any value.
*/

/*!
    \fn bool operator>(const Result<T> &Result, const T &value)

    \relates Result

    Returns true if the \a Result's value is greater than the \a value.

    \note Any value is always greater than an empty Result.
*/


/*!
    \fn bool operator<(const T &value, const Result<T> &Result)

    \relates Result

    Returns true if the \a value is less than the \a Result's value.

    \note Empty Result is always less than any value.
*/

/*!
    \fn bool operator>(const T &value, const Result<T> &Result)

    \relates Result

    Returns true if the \a value is greater than the \a Result's value.

    \note Any value is always greater than an empty Result.
*/

/*!
    \class Error

    This is a helper class that simplifies constructing on an invalid Results.
*/
