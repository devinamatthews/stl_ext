#ifndef _STL_EXT_COMPLEX_HPP_
#define _STL_EXT_COMPLEX_HPP_

#include <complex>

#include "type_traits.hpp"

namespace stl_ext
{

using std::complex;

template <typename T> struct real_type             { typedef T type; };
template <typename T> struct real_type<complex<T>> { typedef T type; };
template <typename T>
using real_type_t = typename real_type<T>::type;

template <typename T> struct complex_type             { typedef complex<T> type; };
template <typename T> struct complex_type<complex<T>> { typedef complex<T> type; };
template <typename T>
using complex_type_t = typename complex_type<T>::type;

template <typename T> struct is_complex             : std::false_type {};
template <typename T> struct is_complex<complex<T>> :  std::true_type {};
template <typename T>
constexpr bool is_complex_v() { return is_complex<T>::value; }
template <typename T, typename U=void>
using enable_if_complex = enable_if<is_complex<T>::value,U>;
template <typename T, typename U=void>
using enable_if_complex_t = typename enable_if_complex<T,U>::type;
template <typename T, typename U=void>
using enable_if_not_complex = enable_if<!is_complex<T>::value,U>;
template <typename T, typename U=void>
using enable_if_not_complex_t = typename enable_if_not_complex<T,U>::type;

}

namespace std
{

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator+(const complex<T>& f, U d)
{
    typedef stl_ext::common_type_t<T,U> V;
    return complex<V>(f)+V(d);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator+(T d, const complex<U>& f)
{
    typedef stl_ext::common_type_t<T,U> V;
    return V(d)+complex<V>(f);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator-(const complex<T>& f, U d)
{
    typedef stl_ext::common_type_t<T,U> V;
    return complex<V>(f)-V(d);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator-(T d, const complex<U>& f)
{
    typedef stl_ext::common_type_t<T,U> V;
    return V(d)-complex<V>(f);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator*(const complex<T>& f, U d)
{
    typedef stl_ext::common_type_t<T,U> V;
    return complex<V>(f)*V(d);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator*(T d, const complex<U>& f)
{
    typedef stl_ext::common_type_t<T,U> V;
    return V(d)*complex<V>(f);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator/(const complex<T>& f, U d)
{
    typedef stl_ext::common_type_t<T,U> V;
    return complex<V>(f)/V(d);
}

template <typename T, typename U>
stl_ext::enable_if_t<is_arithmetic<T>::value &&
            is_arithmetic<U>::value &&
            !is_same<T,U>::value,complex<stl_ext::common_type_t<T,U>>>
operator/(T d, const complex<U>& f)
{
    typedef stl_ext::common_type_t<T,U> V;
    return V(d)/complex<V>(f);
}

}

#endif