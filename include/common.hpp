#pragma once

#ifndef CONSTEXPR
#  if __cplusplus >= 201103L
#    define CONSTEXPR constexpr
#  else
#    define CONSTEXPR inline
#  endif // __cplusplus >= 201103L
#endif // !CONSTEXPR

#ifndef CONSTEXPR_DYNAMIC_ALLOC
#  if __cplusplus >= 201907L
#    define CONSTEXPR_DYNAMIC_ALLOC constexpr
#  else
#    define CONSTEXPR_DYNAMIC_ALLOC inline
#  endif // __cplusplus >= 201907L
#endif // !CONSTEXPR_DYNAMIC_ALLOC

#ifndef NOEXCEPT
#  if __cplusplus >= 201103L
#    define NOEXCEPT noexcept
#  else
#    define NOEXCEPT
#  endif // __cplusplus >= 201907L
#endif // !NOEXCEPT

#ifndef OVERRIDE
#  if __cplusplus >= 201103L
#    define OVERRIDE override
#  else
#    define OVERRIDE
#  endif // __cplusplus >= 201907L
#endif // !OVERRIDE

#ifndef NULLPTR
#  if __cplusplus >= 201103L
#    define NULLPTR nullptr
#  else
#    define NULLPTR NULL
#  endif // __cplusplus >= 201103L
#endif // !NULLPTR
