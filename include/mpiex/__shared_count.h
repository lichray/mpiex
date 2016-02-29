#pragma once

#include "__config.h"

#include <memory>

namespace mpiex
{

using std::allocator_traits;
using std::pointer_traits;

// XXX not thread-safe
class _LIBCPP_TYPE_VIS __shared_count
{
    __shared_count(const __shared_count&);
    __shared_count& operator=(const __shared_count&);

protected:
    long __shared_owners_;
    virtual ~__shared_count();
private:
    virtual void __on_zero_shared() _NOEXCEPT = 0;

public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __shared_count(long __refs = 0) _NOEXCEPT
        : __shared_owners_(__refs) {}

    void __add_shared() _NOEXCEPT;
    bool __release_shared() _NOEXCEPT;
    _LIBCPP_INLINE_VISIBILITY
    long use_count() const _NOEXCEPT {
        return __shared_owners_ + 1;
    }
};

template <class _Alloc>
class __allocator_destructor
{
    typedef allocator_traits<_Alloc> __alloc_traits;
public:
    typedef typename __alloc_traits::pointer pointer;
    typedef typename __alloc_traits::size_type size_type;
private:
    _Alloc& __alloc_;
    size_type __s_;
public:
    _LIBCPP_INLINE_VISIBILITY __allocator_destructor(_Alloc& __a, size_type __s)
             _NOEXCEPT
        : __alloc_(__a), __s_(__s) {}
    _LIBCPP_INLINE_VISIBILITY
    void operator()(pointer __p) _NOEXCEPT
        {__alloc_traits::deallocate(__alloc_, __p, __s_);}
};


// __make_tuple_indices

template <size_t...> struct __tuple_indices {};

template <size_t _Sp, class _IntTuple, size_t _Ep>
struct __make_indices_imp;

template <size_t _Sp, size_t ..._Indices, size_t _Ep>
struct __make_indices_imp<_Sp, __tuple_indices<_Indices...>, _Ep>
{
    typedef typename __make_indices_imp<_Sp+1, __tuple_indices<_Indices..., _Sp>, _Ep>::type type;
};

template <size_t _Ep, size_t ..._Indices>
struct __make_indices_imp<_Ep, __tuple_indices<_Indices...>, _Ep>
{
    typedef __tuple_indices<_Indices...> type;
};

template <size_t _Ep, size_t _Sp = 0>
struct __make_tuple_indices
{
    static_assert(_Sp <= _Ep, "__make_tuple_indices input error");
    typedef typename __make_indices_imp<_Sp, __tuple_indices<>, _Ep>::type type;
};

}
