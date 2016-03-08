// -*- C++ -*-
//===--------------------------- future -----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#pragma once

/*
    future synopsis

namespace std
{

enum class future_errc
{
    future_already_retrieved = 1,
    promise_already_satisfied,
    no_state,
    broken_promise
};

enum class launch
{
    async = 1,
    deferred = 2,
    any = async | deferred
};

enum class future_status
{
    ready,
    timeout,
    deferred
};

template <> struct is_error_code_enum<future_errc> : public true_type { };
error_code make_error_code(future_errc e) noexcept;
error_condition make_error_condition(future_errc e) noexcept;

const error_category& future_category() noexcept;

class future_error
    : public logic_error
{
public:
    future_error(error_code ec);  // exposition only

    const error_code& code() const noexcept;
    const char*       what() const noexcept;
};

template <class R>
class promise
{
public:
    promise();
    template <class Allocator>
        promise(allocator_arg_t, const Allocator& a);
    promise(promise&& rhs) noexcept;
    promise(const promise& rhs) = delete;
    ~promise();

    // assignment
    promise& operator=(promise&& rhs) noexcept;
    promise& operator=(const promise& rhs) = delete;
    void swap(promise& other) noexcept;

    // retrieving the result
    future<R> get_future();

    // setting the result
    void set_value(const R& r);
    void set_value(R&& r);
    void set_exception(exception_ptr p);

    // setting the result with deferred notification
    void set_value_at_thread_exit(const R& r);
    void set_value_at_thread_exit(R&& r);
    void set_exception_at_thread_exit(exception_ptr p);
};

template <class R>
class promise<R&>
{
public:
    promise();
    template <class Allocator>
        promise(allocator_arg_t, const Allocator& a);
    promise(promise&& rhs) noexcept;
    promise(const promise& rhs) = delete;
    ~promise();

    // assignment
    promise& operator=(promise&& rhs) noexcept;
    promise& operator=(const promise& rhs) = delete;
    void swap(promise& other) noexcept;

    // retrieving the result
    future<R&> get_future();

    // setting the result
    void set_value(R& r);
    void set_exception(exception_ptr p);
};

template <>
class promise<void>
{
public:
    promise();
    template <class Allocator>
        promise(allocator_arg_t, const Allocator& a);
    promise(promise&& rhs) noexcept;
    promise(const promise& rhs) = delete;
    ~promise();

    // assignment
    promise& operator=(promise&& rhs) noexcept;
    promise& operator=(const promise& rhs) = delete;
    void swap(promise& other) noexcept;

    // retrieving the result
    future<void> get_future();

    // setting the result
    void set_value();
    void set_exception(exception_ptr p);
};

template <class R> void swap(promise<R>& x, promise<R>& y) noexcept;

template <class R, class Alloc>
    struct uses_allocator<promise<R>, Alloc> : public true_type {};

template <class R>
class future
{
public:
    future() noexcept;
    future(future&&) noexcept;
    future(const future& rhs) = delete;
    ~future();
    future& operator=(const future& rhs) = delete;
    future& operator=(future&&) noexcept;
    shared_future<R> share();

    // retrieving the value
    R get();

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <class R>
class future<R&>
{
public:
    future() noexcept;
    future(future&&) noexcept;
    future(const future& rhs) = delete;
    ~future();
    future& operator=(const future& rhs) = delete;
    future& operator=(future&&) noexcept;
    shared_future<R&> share();

    // retrieving the value
    R& get();

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <>
class future<void>
{
public:
    future() noexcept;
    future(future&&) noexcept;
    future(const future& rhs) = delete;
    ~future();
    future& operator=(const future& rhs) = delete;
    future& operator=(future&&) noexcept;
    shared_future<void> share();

    // retrieving the value
    void get();

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <class R>
class shared_future
{
public:
    shared_future() noexcept;
    shared_future(const shared_future& rhs);
    shared_future(future<R>&&) noexcept;
    shared_future(shared_future&& rhs) noexcept;
    ~shared_future();
    shared_future& operator=(const shared_future& rhs);
    shared_future& operator=(shared_future&& rhs) noexcept;

    // retrieving the value
    const R& get() const;

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <class R>
class shared_future<R&>
{
public:
    shared_future() noexcept;
    shared_future(const shared_future& rhs);
    shared_future(future<R&>&&) noexcept;
    shared_future(shared_future&& rhs) noexcept;
    ~shared_future();
    shared_future& operator=(const shared_future& rhs);
    shared_future& operator=(shared_future&& rhs) noexcept;

    // retrieving the value
    R& get() const;

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <>
class shared_future<void>
{
public:
    shared_future() noexcept;
    shared_future(const shared_future& rhs);
    shared_future(future<void>&&) noexcept;
    shared_future(shared_future&& rhs) noexcept;
    ~shared_future();
    shared_future& operator=(const shared_future& rhs);
    shared_future& operator=(shared_future&& rhs) noexcept;

    // retrieving the value
    void get() const;

    // functions to check state
    bool valid() const noexcept;

    void wait() const;
    template <class Rep, class Period>
        future_status
        wait_for(const chrono::duration<Rep, Period>& rel_time) const;
    template <class Clock, class Duration>
        future_status
        wait_until(const chrono::time_point<Clock, Duration>& abs_time) const;
};

template <class F, class... Args>
  future<typename result_of<typename decay<F>::type(typename decay<Args>::type...)>::type>
  async(F&& f, Args&&... args);

template <class F, class... Args>
  future<typename result_of<typename decay<F>::type(typename decay<Args>::type...)>::type>
  async(launch policy, F&& f, Args&&... args);

template <class> class packaged_task; // undefined

template <class R, class... ArgTypes>
class packaged_task<R(ArgTypes...)>
{
public:
    typedef R result_type;

    // construction and destruction
    packaged_task() noexcept;
    template <class F>
        explicit packaged_task(F&& f);
    template <class F, class Allocator>
        packaged_task(allocator_arg_t, const Allocator& a, F&& f);
    ~packaged_task();

    // no copy
    packaged_task(const packaged_task&) = delete;
    packaged_task& operator=(const packaged_task&) = delete;

    // move support
    packaged_task(packaged_task&& other) noexcept;
    packaged_task& operator=(packaged_task&& other) noexcept;
    void swap(packaged_task& other) noexcept;

    bool valid() const noexcept;

    // result retrieval
    future<R> get_future();

    // execution
    void operator()(ArgTypes... );

    void reset();
};

template <class R>
  void swap(packaged_task<R(ArgTypes...)&, packaged_task<R(ArgTypes...)>&) noexcept;

template <class R, class Alloc> struct uses_allocator<packaged_task<R>, Alloc>;

}  // std

*/

#include "__config.h"
#include "__shared_count.h"
#include "type_traits.h"

#include <mpi.h>

#include <system_error>
#include <memory>
#include <chrono>
#include <exception>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <stdexcept>

namespace mpiex
{

using std::unique_ptr;

using std::lock_guard;
using std::unique_lock;
using std::thread;
using std::mutex;
using std::condition_variable;

using std::future_errc;
using std::future_status;
using std::make_error_code;
using std::future_error;
using std::tuple;

using std::exception_ptr;
using std::rethrow_exception;
using std::current_exception;
using std::runtime_error;

using std::allocator;
using std::allocator_arg;
using std::allocator_arg_t;

namespace chrono = std::chrono;

enum class launch
{
    async = 1,
    deferred = 2,
    any = async | deferred
};

#ifndef _LIBCPP_HAS_NO_STRONG_ENUMS

#ifdef _LIBCXX_UNDERLYING_TYPE
typedef underlying_type<launch>::type __launch_underlying_type;
#else
typedef int __launch_underlying_type;
#endif

inline _LIBCPP_INLINE_VISIBILITY
_LIBCPP_CONSTEXPR
launch
operator&(launch __x, launch __y)
{
    return static_cast<launch>(static_cast<__launch_underlying_type>(__x) &
                               static_cast<__launch_underlying_type>(__y));
}

inline _LIBCPP_INLINE_VISIBILITY
_LIBCPP_CONSTEXPR
launch
operator|(launch __x, launch __y)
{
    return static_cast<launch>(static_cast<__launch_underlying_type>(__x) |
                               static_cast<__launch_underlying_type>(__y));
}

inline _LIBCPP_INLINE_VISIBILITY
_LIBCPP_CONSTEXPR
launch
operator^(launch __x, launch __y)
{
    return static_cast<launch>(static_cast<__launch_underlying_type>(__x) ^
                               static_cast<__launch_underlying_type>(__y));
}

inline _LIBCPP_INLINE_VISIBILITY
_LIBCPP_CONSTEXPR
launch
operator~(launch __x)
{
    return static_cast<launch>(~static_cast<__launch_underlying_type>(__x) & 3);
}

inline _LIBCPP_INLINE_VISIBILITY
launch&
operator&=(launch& __x, launch __y)
{
    __x = __x & __y; return __x;
}

inline _LIBCPP_INLINE_VISIBILITY
launch&
operator|=(launch& __x, launch __y)
{
    __x = __x | __y; return __x;
}

inline _LIBCPP_INLINE_VISIBILITY
launch&
operator^=(launch& __x, launch __y)
{
    __x = __x ^ __y; return __x;
}

#endif  // !_LIBCPP_HAS_NO_STRONG_ENUMS

inline _LIBCPP_ALWAYS_INLINE
void __throw_future_error(future_errc _Ev)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    throw future_error(make_error_code(_Ev));
#else
    assert(!"future_error");
#endif
}

class _LIBCPP_TYPE_VIS __assoc_sub_state
    : public __shared_count
{
protected:
    exception_ptr __exception_;
    mutable mutex __mut_;
    mutable condition_variable __cv_;
    unsigned __state_;

    virtual void __on_zero_shared() _NOEXCEPT;
    void __sub_wait(unique_lock<mutex>& __lk);
public:
    enum
    {
        __constructed = 1,
        __future_attached = 2,
        ready = 4,
        deferred = 8
    };

    _LIBCPP_INLINE_VISIBILITY
    __assoc_sub_state() : __state_(0) {}

    _LIBCPP_INLINE_VISIBILITY
    bool __has_value() const
        {return (__state_ & __constructed) || (__exception_ != nullptr);}

    _LIBCPP_INLINE_VISIBILITY
    void __set_future_attached()
    {
        lock_guard<mutex> __lk(__mut_);
        __state_ |= __future_attached;
    }
    _LIBCPP_INLINE_VISIBILITY
    bool __has_future_attached() const {return (__state_ & __future_attached) != 0;}

    _LIBCPP_INLINE_VISIBILITY
    void __set_deferred() {__state_ |= deferred;}

    void __make_ready();
    _LIBCPP_INLINE_VISIBILITY
    bool is_ready() { return __is_ready(); }
    virtual bool __is_ready() {return (__state_ & ready) != 0;}

    void set_value();

    void set_exception(exception_ptr __p);

    void copy();

    void wait() { __wait(); }
    virtual void __wait();
    template <class _Rep, class _Period>
        future_status
        _LIBCPP_INLINE_VISIBILITY
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const;
    template <class _Clock, class _Duration>
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const;

    virtual void __execute();
};

template <class _Clock, class _Duration>
future_status
__assoc_sub_state::wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
{
    unique_lock<mutex> __lk(__mut_);
    if (__state_ & deferred)
        return future_status::deferred;
    while (!(__state_ & ready) && _Clock::now() < __abs_time)
        __cv_.wait_until(__lk, __abs_time);
    if (__state_ & ready)
        return future_status::ready;
    return future_status::timeout;
}

template <class _Rep, class _Period>
inline
future_status
__assoc_sub_state::wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
{
    return wait_until(chrono::steady_clock::now() + __rel_time);
}

template <class _Rp>
class __assoc_state
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;
    typedef typename aligned_storage<sizeof(_Rp), alignment_of<_Rp>::value>::type _Up;
protected:
    _Up __value_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:

    template <class _Arg>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        void set_value(_Arg&& __arg);
#else
        void set_value(_Arg& __arg);
#endif

    _Rp move();
    typename add_lvalue_reference<_Rp>::type copy();
};

template <class _Rp>
void
__assoc_state<_Rp>::__on_zero_shared() _NOEXCEPT
{
    if (this->__state_ & base::__constructed)
        reinterpret_cast<_Rp*>(&__value_)->~_Rp();
    delete this;
}

template <class _Rp>
template <class _Arg>
void
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
__assoc_state<_Rp>::set_value(_Arg&& __arg)
#else
__assoc_state<_Rp>::set_value(_Arg& __arg)
#endif
{
    unique_lock<mutex> __lk(this->__mut_);
    if (this->__has_value())
        __throw_future_error(future_errc::promise_already_satisfied);
    ::new(&__value_) _Rp(std::forward<_Arg>(__arg));
    this->__state_ |= base::__constructed | base::ready;
    __cv_.notify_all();
}

template <class _Rp>
_Rp
__assoc_state<_Rp>::move()
{
    unique_lock<mutex> __lk(this->__mut_);
    this->__sub_wait(__lk);
    if (this->__exception_ != nullptr)
        rethrow_exception(this->__exception_);
    return std::move(*reinterpret_cast<_Rp*>(&__value_));
}

template <class _Rp>
typename add_lvalue_reference<_Rp>::type
__assoc_state<_Rp>::copy()
{
    unique_lock<mutex> __lk(this->__mut_);
    this->__sub_wait(__lk);
    if (this->__exception_ != nullptr)
        rethrow_exception(this->__exception_);
    return *reinterpret_cast<_Rp*>(&__value_);
}

template <class _Rp>
class __assoc_state<_Rp&>
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;
    typedef _Rp* _Up;
protected:
    _Up __value_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:

    void set_value(_Rp& __arg);

    _Rp& copy();
};

template <class _Rp>
void
__assoc_state<_Rp&>::__on_zero_shared() _NOEXCEPT
{
    delete this;
}

template <class _Rp>
void
__assoc_state<_Rp&>::set_value(_Rp& __arg)
{
    unique_lock<mutex> __lk(this->__mut_);
    if (this->__has_value())
        __throw_future_error(future_errc::promise_already_satisfied);
    __value_ = std::addressof(__arg);
    this->__state_ |= base::__constructed | base::ready;
    __cv_.notify_all();
}

template <class _Rp>
_Rp&
__assoc_state<_Rp&>::copy()
{
    unique_lock<mutex> __lk(this->__mut_);
    this->__sub_wait(__lk);
    if (this->__exception_ != nullptr)
        rethrow_exception(this->__exception_);
    return *__value_;
}

template <class _Rp, class _Alloc>
class __assoc_state_alloc
    : public __assoc_state<_Rp>
{
    typedef __assoc_state<_Rp> base;
    _Alloc __alloc_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __assoc_state_alloc(const _Alloc& __a)
        : __alloc_(__a) {}
};

template <class _Rp, class _Alloc>
void
__assoc_state_alloc<_Rp, _Alloc>::__on_zero_shared() _NOEXCEPT
{
    if (this->__state_ & base::__constructed)
        reinterpret_cast<_Rp*>(std::addressof(this->__value_))->~_Rp();
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<__assoc_state_alloc> _Al;
    typedef allocator_traits<_Al> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;
    _Al __a(__alloc_);
    this->~__assoc_state_alloc();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template <class _Rp, class _Alloc>
class __assoc_state_alloc<_Rp&, _Alloc>
    : public __assoc_state<_Rp&>
{
    typedef __assoc_state<_Rp&> base;
    _Alloc __alloc_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __assoc_state_alloc(const _Alloc& __a)
        : __alloc_(__a) {}
};

template <class _Rp, class _Alloc>
void
__assoc_state_alloc<_Rp&, _Alloc>::__on_zero_shared() _NOEXCEPT
{
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<__assoc_state_alloc> _Al;
    typedef allocator_traits<_Al> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;
    _Al __a(__alloc_);
    this->~__assoc_state_alloc();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template <class _Alloc>
class __assoc_sub_state_alloc
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;
    _Alloc __alloc_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __assoc_sub_state_alloc(const _Alloc& __a)
        : __alloc_(__a) {}
};

template <class _Alloc>
void
__assoc_sub_state_alloc<_Alloc>::__on_zero_shared() _NOEXCEPT
{
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<__assoc_state_alloc> _Al;
    typedef allocator_traits<_Al> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;
    _Al __a(__alloc_);
    this->~__assoc_sub_state_alloc();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template <class _Rp, class _Fp>
class __deferred_assoc_state
    : public __assoc_state<_Rp>
{
    typedef __assoc_state<_Rp> base;

    _Fp __func_;

public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __deferred_assoc_state(_Fp&& __f);
#endif

    virtual void __execute();
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
inline
__deferred_assoc_state<_Rp, _Fp>::__deferred_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
    this->__set_deferred();
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
void
__deferred_assoc_state<_Rp, _Fp>::__execute()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        this->set_value(__func_());
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        this->set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template <class _Fp>
class __deferred_assoc_state<void, _Fp>
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;

    _Fp __func_;

public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __deferred_assoc_state(_Fp&& __f);
#endif

    virtual void __execute();
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
inline
__deferred_assoc_state<void, _Fp>::__deferred_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
    this->__set_deferred();
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
void
__deferred_assoc_state<void, _Fp>::__execute()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        __func_();
        this->set_value();
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        this->set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template <class _Rp, class _Fp>
class __async_assoc_state
    : public __assoc_state<_Rp>
{
    typedef __assoc_state<_Rp> base;

    _Fp __func_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __async_assoc_state(_Fp&& __f);
#endif

    virtual void __execute();
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
inline
__async_assoc_state<_Rp, _Fp>::__async_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
void
__async_assoc_state<_Rp, _Fp>::__execute()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        this->set_value(__func_());
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        this->set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template <class _Rp, class _Fp>
void
__async_assoc_state<_Rp, _Fp>::__on_zero_shared() _NOEXCEPT
{
    this->wait();
    base::__on_zero_shared();
}

template <class _Fp>
class __async_assoc_state<void, _Fp>
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;

    _Fp __func_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __async_assoc_state(_Fp&& __f);
#endif

    virtual void __execute();
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
inline
__async_assoc_state<void, _Fp>::__async_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
void
__async_assoc_state<void, _Fp>::__execute()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        __func_();
        this->set_value();
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        this->set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template <class _Fp>
void
__async_assoc_state<void, _Fp>::__on_zero_shared() _NOEXCEPT
{
    this->wait();
    base::__on_zero_shared();
}

// __mpi_assoc_state

template <class _Rp, class _Fp>
class __mpi_assoc_state
    : public __assoc_state<_Rp>
{
    typedef __assoc_state<_Rp> base;

    _Fp __func_;
    MPI_Request __req_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __mpi_assoc_state(_Fp&& __f);
#endif

    virtual bool __is_ready() override;
    virtual void __wait() override;
    virtual void __execute() override;
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
inline
__mpi_assoc_state<_Rp, _Fp>::__mpi_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp, class _Fp>
void
__mpi_assoc_state<_Rp, _Fp>::__execute()
{
    ::new(std::addressof(this->__value_)) _Rp;  // default ctor
    __req_ = __func_(*reinterpret_cast<_Rp*>(std::addressof(this->__value_)));
}

template <class _Rp, class _Fp>
void
__mpi_assoc_state<_Rp, _Fp>::__on_zero_shared() _NOEXCEPT
{
    this->__wait();
    base::__on_zero_shared();
}

template <class _Rp, class _Fp>
void
__mpi_assoc_state<_Rp, _Fp>::__wait()
{
    MPI_Status st;
    // not thread-safe
    // XXX needs polish
    if (MPI_Wait(&__req_, &st) == MPI_SUCCESS)
        this->__state_ |= base::__constructed | base::ready;
    else
        this->set_exception(make_exception_ptr(runtime_error("MPI_Wait")));
}

template <class _Rp, class _Fp>
bool
__mpi_assoc_state<_Rp, _Fp>::__is_ready()
{
    MPI_Status st;
    int finished;
    MPI_Test(&__req_, &finished, MPI_STATUS_IGNORE);
    return finished;
}

template <class _Fp>
class __mpi_assoc_state<void, _Fp>
    : public __assoc_sub_state
{
    typedef __assoc_sub_state base;

    _Fp __func_;
    MPI_Request __req_;

    virtual void __on_zero_shared() _NOEXCEPT;
public:
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    explicit __mpi_assoc_state(_Fp&& __f);
#endif

    virtual bool __is_ready() override;
    virtual void __wait() override;
    virtual void __execute();
};

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
inline
__mpi_assoc_state<void, _Fp>::__mpi_assoc_state(_Fp&& __f)
    : __func_(std::forward<_Fp>(__f))
{
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Fp>
void
__mpi_assoc_state<void, _Fp>::__execute()
{
    __req_ = __func_();
}

template <class _Fp>
void
__mpi_assoc_state<void, _Fp>::__on_zero_shared() _NOEXCEPT
{
    this->__wait();
    base::__on_zero_shared();
}

template <class _Fp>
void
__mpi_assoc_state<void, _Fp>::__wait()
{
    MPI_Status st;
    // not thread-safe
    // XXX needs polish
    if (MPI_Wait(&__req_, &st) == MPI_SUCCESS)
        __state_ |= __constructed | ready;
    else
        this->set_exception(make_exception_ptr(runtime_error("MPI_Wait")));
}

template <class _Fp>
bool
__mpi_assoc_state<void, _Fp>::__is_ready()
{
    MPI_Status st;
    int finished;
    MPI_Test(&__req_, &finished, MPI_STATUS_IGNORE);
    return finished;
}

template <class _Rp> class _LIBCPP_TYPE_VIS_ONLY promise;
template <class _Rp> class _LIBCPP_TYPE_VIS_ONLY shared_future;

// future

template <class _Rp> class _LIBCPP_TYPE_VIS_ONLY future;

template <class _Rp, class _Fp>
future<_Rp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
__make_deferred_assoc_state(_Fp&& __f);
#else
__make_deferred_assoc_state(_Fp __f);
#endif

template <class _Rp, class _Fp>
future<_Rp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
__make_async_assoc_state(_Fp&& __f);
#else
__make_async_assoc_state(_Fp __f);
#endif

template <class _Rp, class _Fp>
future<_Rp>
__make_mpi_assoc_state(_Fp&& __f);

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY future
{
    __assoc_state<_Rp>* __state_;

    explicit future(__assoc_state<_Rp>* __state);

    template <class> friend class promise;
    template <class> friend class shared_future;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp&& __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp&& __f);
#else
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp __f);
#endif

    template <class _R1, class _Fp>
        friend future<_R1> __make_mpi_assoc_state(_Fp&& __f);

public:
    _LIBCPP_INLINE_VISIBILITY
    future() _NOEXCEPT : __state_(nullptr) {}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    future(future&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    future(const future&) = delete;
    future& operator=(const future&) = delete;
    _LIBCPP_INLINE_VISIBILITY
    future& operator=(future&& __rhs) _NOEXCEPT
        {
            future(std::move(__rhs)).swap(*this);
            return *this;
        }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    future(const future&);
    future& operator=(const future&);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~future();
    _LIBCPP_INLINE_VISIBILITY
    shared_future<_Rp> share();

    // retrieving the value
    _Rp get();

    _LIBCPP_INLINE_VISIBILITY
    void swap(future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
future<_Rp>::future(__assoc_state<_Rp>* __state)
    : __state_(__state)
{
    if (__state_->__has_future_attached())
        __throw_future_error(future_errc::future_already_retrieved);
    __state_->__add_shared();
    __state_->__set_future_attached();
}

struct __release_shared_count
{
    void operator()(__shared_count* p) {p->__release_shared();}
};

template <class _Rp>
future<_Rp>::~future()
{
    if (__state_)
        __state_->__release_shared();
}

template <class _Rp>
_Rp
future<_Rp>::get()
{
    unique_ptr<__shared_count, __release_shared_count> __(__state_);
    __assoc_state<_Rp>* __s = __state_;
    __state_ = nullptr;
    return __s->move();
}

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY future<_Rp&>
{
    __assoc_state<_Rp&>* __state_;

    explicit future(__assoc_state<_Rp&>* __state);

    template <class> friend class promise;
    template <class> friend class shared_future;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp&& __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp&& __f);
#else
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp __f);
#endif

    template <class _R1, class _Fp>
        friend future<_R1> __make_mpi_assoc_state(_Fp&& __f);

public:
    _LIBCPP_INLINE_VISIBILITY
    future() _NOEXCEPT : __state_(nullptr) {}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    future(future&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    future(const future&) = delete;
    future& operator=(const future&) = delete;
    _LIBCPP_INLINE_VISIBILITY
    future& operator=(future&& __rhs) _NOEXCEPT
        {
            future(std::move(__rhs)).swap(*this);
            return *this;
        }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    future(const future&);
    future& operator=(const future&);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~future();
    _LIBCPP_INLINE_VISIBILITY
    shared_future<_Rp&> share();

    // retrieving the value
    _Rp& get();

    _LIBCPP_INLINE_VISIBILITY
    void swap(future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
future<_Rp&>::future(__assoc_state<_Rp&>* __state)
    : __state_(__state)
{
    if (__state_->__has_future_attached())
        __throw_future_error(future_errc::future_already_retrieved);
    __state_->__add_shared();
    __state_->__set_future_attached();
}

template <class _Rp>
future<_Rp&>::~future()
{
    if (__state_)
        __state_->__release_shared();
}

template <class _Rp>
_Rp&
future<_Rp&>::get()
{
    unique_ptr<__shared_count, __release_shared_count> __(__state_);
    __assoc_state<_Rp&>* __s = __state_;
    __state_ = nullptr;
    return __s->copy();
}

template <>
class _LIBCPP_TYPE_VIS future<void>
{
    __assoc_sub_state* __state_;

    explicit future(__assoc_sub_state* __state);

    template <class> friend class promise;
    template <class> friend class shared_future;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp&& __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp&& __f);
#else
    template <class _R1, class _Fp>
        friend future<_R1> __make_deferred_assoc_state(_Fp __f);
    template <class _R1, class _Fp>
        friend future<_R1> __make_async_assoc_state(_Fp __f);
#endif

    template <class _R1, class _Fp>
        friend future<_R1> __make_mpi_assoc_state(_Fp&& __f);

public:
    _LIBCPP_INLINE_VISIBILITY
    future() _NOEXCEPT : __state_(nullptr) {}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    future(future&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    future(const future&) = delete;
    future& operator=(const future&) = delete;
    _LIBCPP_INLINE_VISIBILITY
    future& operator=(future&& __rhs) _NOEXCEPT
        {
            future(std::move(__rhs)).swap(*this);
            return *this;
        }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    future(const future&);
    future& operator=(const future&);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~future();
    _LIBCPP_INLINE_VISIBILITY
    shared_future<void> share();

    // retrieving the value
    void get();

    _LIBCPP_INLINE_VISIBILITY
    void swap(future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(future<_Rp>& __x, future<_Rp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

// promise<R>

template <class _Callable> class packaged_task;

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY promise
{
    __assoc_state<_Rp>* __state_;

    _LIBCPP_INLINE_VISIBILITY
    explicit promise(nullptr_t) _NOEXCEPT : __state_(nullptr) {}

    template <class> friend class packaged_task;
public:
    promise();
    template <class _Alloc>
        promise(allocator_arg_t, const _Alloc& __a);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise(promise&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    promise(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~promise();

    // assignment
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise& operator=(promise&& __rhs) _NOEXCEPT
        {
            promise(std::move(__rhs)).swap(*this);
            return *this;
        }
    promise& operator=(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise& operator=(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    void swap(promise& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // retrieving the result
    future<_Rp> get_future();

    // setting the result
    void set_value(const _Rp& __r);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    void set_value(_Rp&& __r);
#endif
    void set_exception(exception_ptr __p);
};

template <class _Rp>
promise<_Rp>::promise()
    : __state_(new __assoc_state<_Rp>)
{
}

template <class _Rp>
template <class _Alloc>
promise<_Rp>::promise(allocator_arg_t, const _Alloc& __a0)
{
    typedef __assoc_state_alloc<_Rp, _Alloc> _State;
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<_State> _A2;
    typedef __allocator_destructor<_A2> _D2;
    _A2 __a(__a0);
    unique_ptr<_State, _D2> __hold(__a.allocate(1), _D2(__a, 1));
    ::new(static_cast<void*>(std::addressof(*__hold.get()))) _State(__a0);
    __state_ = std::addressof(*__hold.release());
}

template <class _Rp>
promise<_Rp>::~promise()
{
    if (__state_)
    {
        if (!__state_->__has_value() && __state_->use_count() > 1)
            __state_->set_exception(make_exception_ptr(
                      future_error(make_error_code(future_errc::broken_promise))
                                                      ));
        __state_->__release_shared();
    }
}

template <class _Rp>
future<_Rp>
promise<_Rp>::get_future()
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    return future<_Rp>(__state_);
}

template <class _Rp>
void
promise<_Rp>::set_value(const _Rp& __r)
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    __state_->set_value(__r);
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp>
void
promise<_Rp>::set_value(_Rp&& __r)
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    __state_->set_value(std::move(__r));
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class _Rp>
void
promise<_Rp>::set_exception(exception_ptr __p)
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    __state_->set_exception(__p);
}

// promise<R&>

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY promise<_Rp&>
{
    __assoc_state<_Rp&>* __state_;

    _LIBCPP_INLINE_VISIBILITY
    explicit promise(nullptr_t) _NOEXCEPT : __state_(nullptr) {}

    template <class> friend class packaged_task;

public:
    promise();
    template <class _Allocator>
        promise(allocator_arg_t, const _Allocator& __a);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise(promise&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    promise(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~promise();

    // assignment
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise& operator=(promise&& __rhs) _NOEXCEPT
        {
            promise(std::move(__rhs)).swap(*this);
            return *this;
        }
    promise& operator=(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise& operator=(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    void swap(promise& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // retrieving the result
    future<_Rp&> get_future();

    // setting the result
    void set_value(_Rp& __r);
    void set_exception(exception_ptr __p);
};

template <class _Rp>
promise<_Rp&>::promise()
    : __state_(new __assoc_state<_Rp&>)
{
}

template <class _Rp>
template <class _Alloc>
promise<_Rp&>::promise(allocator_arg_t, const _Alloc& __a0)
{
    typedef __assoc_state_alloc<_Rp&, _Alloc> _State;
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<_State> _A2;
    typedef __allocator_destructor<_A2> _D2;
    _A2 __a(__a0);
    unique_ptr<_State, _D2> __hold(__a.allocate(1), _D2(__a, 1));
    ::new(static_cast<void*>(std::addressof(*__hold.get()))) _State(__a0);
    __state_ = std::addressof(*__hold.release());
}

template <class _Rp>
promise<_Rp&>::~promise()
{
    if (__state_)
    {
        if (!__state_->__has_value() && __state_->use_count() > 1)
            __state_->set_exception(make_exception_ptr(
                      future_error(make_error_code(future_errc::broken_promise))
                                                      ));
        __state_->__release_shared();
    }
}

template <class _Rp>
future<_Rp&>
promise<_Rp&>::get_future()
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    return future<_Rp&>(__state_);
}

template <class _Rp>
void
promise<_Rp&>::set_value(_Rp& __r)
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    __state_->set_value(__r);
}

template <class _Rp>
void
promise<_Rp&>::set_exception(exception_ptr __p)
{
    if (__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    __state_->set_exception(__p);
}

// promise<void>

template <>
class _LIBCPP_TYPE_VIS promise<void>
{
    __assoc_sub_state* __state_;

    _LIBCPP_INLINE_VISIBILITY
    explicit promise(nullptr_t) _NOEXCEPT : __state_(nullptr) {}

    template <class> friend class packaged_task;

public:
    promise();
    template <class _Allocator>
        promise(allocator_arg_t, const _Allocator& __a);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise(promise&& __rhs) _NOEXCEPT
        : __state_(__rhs.__state_) {__rhs.__state_ = nullptr;}
    promise(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~promise();

    // assignment
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    promise& operator=(promise&& __rhs) _NOEXCEPT
        {
            promise(std::move(__rhs)).swap(*this);
            return *this;
        }
    promise& operator=(const promise& __rhs) = delete;
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
private:
    promise& operator=(const promise& __rhs);
public:
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    void swap(promise& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // retrieving the result
    future<void> get_future();

    // setting the result
    void set_value();
    void set_exception(exception_ptr __p);
};

template <class _Alloc>
promise<void>::promise(allocator_arg_t, const _Alloc& __a0)
{
    typedef __assoc_sub_state_alloc<_Alloc> _State;
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<_State> _A2;
    typedef __allocator_destructor<_A2> _D2;
    _A2 __a(__a0);
    unique_ptr<_State, _D2> __hold(__a.allocate(1), _D2(__a, 1));
    ::new(static_cast<void*>(std::addressof(*__hold.get()))) _State(__a0);
    __state_ = std::addressof(*__hold.release());
}

template <class _Rp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(promise<_Rp>& __x, promise<_Rp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

#ifndef _LIBCPP_HAS_NO_VARIADICS

// packaged_task

template<class _Fp> class __packaged_task_base;

template<class _Rp, class ..._ArgTypes>
class __packaged_task_base<_Rp(_ArgTypes...)>
{
    __packaged_task_base(const __packaged_task_base&);
    __packaged_task_base& operator=(const __packaged_task_base&);
public:
    _LIBCPP_INLINE_VISIBILITY
    __packaged_task_base() {}
    _LIBCPP_INLINE_VISIBILITY
    virtual ~__packaged_task_base() {}
    virtual void __move_to(__packaged_task_base*) _NOEXCEPT = 0;
    virtual void destroy() = 0;
    virtual void destroy_deallocate() = 0;
    virtual _Rp operator()(_ArgTypes&& ...) = 0;
};

template<class _FD, class _Alloc, class _FB> class __packaged_task_func;

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
class __packaged_task_func<_Fp, _Alloc, _Rp(_ArgTypes...)>
    : public  __packaged_task_base<_Rp(_ArgTypes...)>
{
    tuple<_Fp, _Alloc> __f_;
public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __packaged_task_func(const _Fp& __f) : __f_(__f) {}
    _LIBCPP_INLINE_VISIBILITY
    explicit __packaged_task_func(_Fp&& __f) : __f_(std::move(__f)) {}
    _LIBCPP_INLINE_VISIBILITY
    __packaged_task_func(const _Fp& __f, const _Alloc& __a)
        : __f_(__f, __a) {}
    _LIBCPP_INLINE_VISIBILITY
    __packaged_task_func(_Fp&& __f, const _Alloc& __a)
        : __f_(std::move(__f), __a) {}
    virtual void __move_to(__packaged_task_base<_Rp(_ArgTypes...)>*) _NOEXCEPT;
    virtual void destroy();
    virtual void destroy_deallocate();
    virtual _Rp operator()(_ArgTypes&& ... __args);
};

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
void
__packaged_task_func<_Fp, _Alloc, _Rp(_ArgTypes...)>::__move_to(
                              __packaged_task_base<_Rp(_ArgTypes...)>* __p) _NOEXCEPT
{
    ::new (__p) __packaged_task_func(std::move(std::get<0>(__f_)), std::move(std::get<1>(__f_)));
}

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
void
__packaged_task_func<_Fp, _Alloc, _Rp(_ArgTypes...)>::destroy()
{
    __f_.~tuple<_Fp, _Alloc>();
}

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
void
__packaged_task_func<_Fp, _Alloc, _Rp(_ArgTypes...)>::destroy_deallocate()
{
    typedef typename allocator_traits<_Alloc>::template rebind_alloc<__packaged_task_func> _Ap;
    typedef allocator_traits<_Ap> _ATraits;
    typedef pointer_traits<typename _ATraits::pointer> _PTraits;
    _Ap __a(std::get<1>(__f_));
    __f_.~tuple<_Fp, _Alloc>();
    __a.deallocate(_PTraits::pointer_to(*this), 1);
}

template<class _Fp, class _Alloc, class _Rp, class ..._ArgTypes>
_Rp
__packaged_task_func<_Fp, _Alloc, _Rp(_ArgTypes...)>::operator()(_ArgTypes&& ... __arg)
{
    return __invoke(std::get<0>(__f_), std::forward<_ArgTypes>(__arg)...);
}

template <class _Callable> class __packaged_task_function;

template<class _Rp, class ..._ArgTypes>
class __packaged_task_function<_Rp(_ArgTypes...)>
{
    typedef __packaged_task_base<_Rp(_ArgTypes...)> __base;
    typename aligned_storage<3*sizeof(void*)>::type __buf_;
    __base* __f_;

public:
    typedef _Rp result_type;

    // construct/copy/destroy:
    _LIBCPP_INLINE_VISIBILITY
    __packaged_task_function() _NOEXCEPT : __f_(nullptr) {}
    template<class _Fp>
      __packaged_task_function(_Fp&& __f);
    template<class _Fp, class _Alloc>
      __packaged_task_function(allocator_arg_t, const _Alloc& __a, _Fp&& __f);

    __packaged_task_function(__packaged_task_function&&) _NOEXCEPT;
    __packaged_task_function& operator=(__packaged_task_function&&) _NOEXCEPT;

    __packaged_task_function(const __packaged_task_function&) =  delete;
    __packaged_task_function& operator=(const __packaged_task_function&) =  delete;

    ~__packaged_task_function();

    void swap(__packaged_task_function&) _NOEXCEPT;

    _LIBCPP_INLINE_VISIBILITY
    _Rp operator()(_ArgTypes...) const;
};

template<class _Rp, class ..._ArgTypes>
__packaged_task_function<_Rp(_ArgTypes...)>::__packaged_task_function(__packaged_task_function&& __f) _NOEXCEPT
{
    if (__f.__f_ == nullptr)
        __f_ = nullptr;
    else if (__f.__f_ == (__base*)&__f.__buf_)
    {
        __f_ = (__base*)&__buf_;
        __f.__f_->__move_to(__f_);
    }
    else
    {
        __f_ = __f.__f_;
        __f.__f_ = nullptr;
    }
}

template<class _Rp, class ..._ArgTypes>
template <class _Fp>
__packaged_task_function<_Rp(_ArgTypes...)>::__packaged_task_function(_Fp&& __f)
    : __f_(nullptr)
{
    typedef typename remove_reference<typename decay<_Fp>::type>::type _FR;
    typedef __packaged_task_func<_FR, allocator<_FR>, _Rp(_ArgTypes...)> _FF;
    if (sizeof(_FF) <= sizeof(__buf_))
    {
        __f_ = (__base*)&__buf_;
        ::new (__f_) _FF(std::forward<_Fp>(__f));
    }
    else
    {
        typedef allocator<_FF> _Ap;
        _Ap __a;
        typedef __allocator_destructor<_Ap> _Dp;
        unique_ptr<__base, _Dp> __hold(__a.allocate(1), _Dp(__a, 1));
        ::new (__hold.get()) _FF(std::forward<_Fp>(__f), allocator<_FR>(__a));
        __f_ = __hold.release();
    }
}

template<class _Rp, class ..._ArgTypes>
template <class _Fp, class _Alloc>
__packaged_task_function<_Rp(_ArgTypes...)>::__packaged_task_function(
                                  allocator_arg_t, const _Alloc& __a0, _Fp&& __f)
    : __f_(nullptr)
{
    typedef typename remove_reference<typename decay<_Fp>::type>::type _FR;
    typedef __packaged_task_func<_FR, _Alloc, _Rp(_ArgTypes...)> _FF;
    if (sizeof(_FF) <= sizeof(__buf_))
    {
        __f_ = (__base*)&__buf_;
        ::new (__f_) _FF(std::forward<_Fp>(__f));
    }
    else
    {
        typedef typename allocator_traits<_Alloc>::template rebind_alloc<_FF> _Ap;
        _Ap __a(__a0);
        typedef __allocator_destructor<_Ap> _Dp;
        unique_ptr<__base, _Dp> __hold(__a.allocate(1), _Dp(__a, 1));
        ::new (static_cast<void*>(std::addressof(*__hold.get())))
            _FF(std::forward<_Fp>(__f), _Alloc(__a));
        __f_ = std::addressof(*__hold.release());
    }
}

template<class _Rp, class ..._ArgTypes>
__packaged_task_function<_Rp(_ArgTypes...)>&
__packaged_task_function<_Rp(_ArgTypes...)>::operator=(__packaged_task_function&& __f) _NOEXCEPT
{
    if (__f_ == (__base*)&__buf_)
        __f_->destroy();
    else if (__f_)
        __f_->destroy_deallocate();
    __f_ = nullptr;
    if (__f.__f_ == nullptr)
        __f_ = nullptr;
    else if (__f.__f_ == (__base*)&__f.__buf_)
    {
        __f_ = (__base*)&__buf_;
        __f.__f_->__move_to(__f_);
    }
    else
    {
        __f_ = __f.__f_;
        __f.__f_ = nullptr;
    }
    return *this;
}

template<class _Rp, class ..._ArgTypes>
__packaged_task_function<_Rp(_ArgTypes...)>::~__packaged_task_function()
{
    if (__f_ == (__base*)&__buf_)
        __f_->destroy();
    else if (__f_)
        __f_->destroy_deallocate();
}

template<class _Rp, class ..._ArgTypes>
void
__packaged_task_function<_Rp(_ArgTypes...)>::swap(__packaged_task_function& __f) _NOEXCEPT
{
    if (__f_ == (__base*)&__buf_ && __f.__f_ == (__base*)&__f.__buf_)
    {
        typename aligned_storage<sizeof(__buf_)>::type __tempbuf;
        __base* __t = (__base*)&__tempbuf;
        __f_->__move_to(__t);
        __f_->destroy();
        __f_ = nullptr;
        __f.__f_->__move_to((__base*)&__buf_);
        __f.__f_->destroy();
        __f.__f_ = nullptr;
        __f_ = (__base*)&__buf_;
        __t->__move_to((__base*)&__f.__buf_);
        __t->destroy();
        __f.__f_ = (__base*)&__f.__buf_;
    }
    else if (__f_ == (__base*)&__buf_)
    {
        __f_->__move_to((__base*)&__f.__buf_);
        __f_->destroy();
        __f_ = __f.__f_;
        __f.__f_ = (__base*)&__f.__buf_;
    }
    else if (__f.__f_ == (__base*)&__f.__buf_)
    {
        __f.__f_->__move_to((__base*)&__buf_);
        __f.__f_->destroy();
        __f.__f_ = __f_;
        __f_ = (__base*)&__buf_;
    }
    else
        std::swap(__f_, __f.__f_);
}

template<class _Rp, class ..._ArgTypes>
inline
_Rp
__packaged_task_function<_Rp(_ArgTypes...)>::operator()(_ArgTypes... __arg) const
{
    return (*__f_)(std::forward<_ArgTypes>(__arg)...);
}

template<class _Rp, class ..._ArgTypes>
class _LIBCPP_TYPE_VIS_ONLY packaged_task<_Rp(_ArgTypes...)>
{
public:
    typedef _Rp result_type;

private:
    __packaged_task_function<result_type(_ArgTypes...)> __f_;
    promise<result_type>                                __p_;

public:
    // construction and destruction
    _LIBCPP_INLINE_VISIBILITY
    packaged_task() _NOEXCEPT : __p_(nullptr) {}
    template <class _Fp,
              class = typename enable_if
              <
                  !is_same<
                      typename decay<_Fp>::type, 
                      packaged_task
                      >::value
                  >::type
             >
        _LIBCPP_INLINE_VISIBILITY
        explicit packaged_task(_Fp&& __f) : __f_(std::forward<_Fp>(__f)) {}
    template <class _Fp, class _Allocator,
              class = typename enable_if
              <
                  !is_same<
                      typename decay<_Fp>::type, 
                      packaged_task
                      >::value
                  >::type
              >
        _LIBCPP_INLINE_VISIBILITY
        packaged_task(allocator_arg_t, const _Allocator& __a, _Fp&& __f)
             : __f_(allocator_arg, __a, std::forward<_Fp>(__f)),
               __p_(allocator_arg, __a) {}
    // ~packaged_task() = default;

    // no copy
    packaged_task(const packaged_task&) = delete;
    packaged_task& operator=(const packaged_task&) = delete;

    // move support
    _LIBCPP_INLINE_VISIBILITY
    packaged_task(packaged_task&& __other) _NOEXCEPT
        : __f_(std::move(__other.__f_)), __p_(std::move(__other.__p_)) {}
    _LIBCPP_INLINE_VISIBILITY
    packaged_task& operator=(packaged_task&& __other) _NOEXCEPT
    {
        __f_ = std::move(__other.__f_);
        __p_ = std::move(__other.__p_);
        return *this;
    }
    _LIBCPP_INLINE_VISIBILITY
    void swap(packaged_task& __other) _NOEXCEPT
    {
        __f_.swap(__other.__f_);
        __p_.swap(__other.__p_);
    }

    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __p_.__state_ != nullptr;}

    // result retrieval
    _LIBCPP_INLINE_VISIBILITY
    future<result_type> get_future() {return __p_.get_future();}

    // execution
    void operator()(_ArgTypes... __args);

    void reset();
};

template<class _Rp, class ..._ArgTypes>
void
packaged_task<_Rp(_ArgTypes...)>::operator()(_ArgTypes... __args)
{
    if (__p_.__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    if (__p_.__state_->__has_value())
        __throw_future_error(future_errc::promise_already_satisfied);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        __p_.set_value(__f_(std::forward<_ArgTypes>(__args)...));
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __p_.set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class _Rp, class ..._ArgTypes>
void
packaged_task<_Rp(_ArgTypes...)>::reset()
{
    if (!valid())
        __throw_future_error(future_errc::no_state);
    __p_ = promise<result_type>();
}

template<class ..._ArgTypes>
class _LIBCPP_TYPE_VIS_ONLY packaged_task<void(_ArgTypes...)>
{
public:
    typedef void result_type;

private:
    __packaged_task_function<result_type(_ArgTypes...)> __f_;
    promise<result_type>                                __p_;

public:
    // construction and destruction
    _LIBCPP_INLINE_VISIBILITY
    packaged_task() _NOEXCEPT : __p_(nullptr) {}
    template <class _Fp,
              class = typename enable_if
              <
                  !is_same<
                      typename decay<_Fp>::type, 
                      packaged_task
                      >::value
                  >::type
              >
        _LIBCPP_INLINE_VISIBILITY
        explicit packaged_task(_Fp&& __f) : __f_(std::forward<_Fp>(__f)) {}
    template <class _Fp, class _Allocator,
              class = typename enable_if
              <
                  !is_same<
                      typename decay<_Fp>::type, 
                      packaged_task
                      >::value
                  >::type
              >    
        _LIBCPP_INLINE_VISIBILITY
        packaged_task(allocator_arg_t, const _Allocator& __a, _Fp&& __f)
             : __f_(allocator_arg, __a, std::forward<_Fp>(__f)),
               __p_(allocator_arg, __a) {}
    // ~packaged_task() = default;

    // no copy
    packaged_task(const packaged_task&) = delete;
    packaged_task& operator=(const packaged_task&) = delete;

    // move support
    _LIBCPP_INLINE_VISIBILITY
    packaged_task(packaged_task&& __other) _NOEXCEPT
        : __f_(std::move(__other.__f_)), __p_(std::move(__other.__p_)) {}
    _LIBCPP_INLINE_VISIBILITY
    packaged_task& operator=(packaged_task&& __other) _NOEXCEPT
    {
        __f_ = std::move(__other.__f_);
        __p_ = std::move(__other.__p_);
        return *this;
    }
    _LIBCPP_INLINE_VISIBILITY
    void swap(packaged_task& __other) _NOEXCEPT
    {
        __f_.swap(__other.__f_);
        __p_.swap(__other.__p_);
    }

    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __p_.__state_ != nullptr;}

    // result retrieval
    _LIBCPP_INLINE_VISIBILITY
    future<result_type> get_future() {return __p_.get_future();}

    // execution
    void operator()(_ArgTypes... __args);

    void reset();
};

template<class ..._ArgTypes>
void
packaged_task<void(_ArgTypes...)>::operator()(_ArgTypes... __args)
{
    if (__p_.__state_ == nullptr)
        __throw_future_error(future_errc::no_state);
    if (__p_.__state_->__has_value())
        __throw_future_error(future_errc::promise_already_satisfied);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        __f_(std::forward<_ArgTypes>(__args)...);
        __p_.set_value();
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        __p_.set_exception(current_exception());
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
}

template<class ..._ArgTypes>
void
packaged_task<void(_ArgTypes...)>::reset()
{
    if (!valid())
        __throw_future_error(future_errc::no_state);
    __p_ = promise<result_type>();
}

template <class _Callable>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(packaged_task<_Callable>& __x, packaged_task<_Callable>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

template <class _Rp, class _Fp>
future<_Rp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
__make_deferred_assoc_state(_Fp&& __f)
#else
__make_deferred_assoc_state(_Fp __f)
#endif
{
    unique_ptr<__deferred_assoc_state<_Rp, _Fp>, __release_shared_count>
        __h(new __deferred_assoc_state<_Rp, _Fp>(std::forward<_Fp>(__f)));
    return future<_Rp>(__h.get());
}

template <class _Rp, class _Fp>
future<_Rp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
__make_async_assoc_state(_Fp&& __f)
#else
__make_async_assoc_state(_Fp __f)
#endif
{
    unique_ptr<__async_assoc_state<_Rp, _Fp>, __release_shared_count>
        __h(new __async_assoc_state<_Rp, _Fp>(std::forward<_Fp>(__f)));
    std::thread(&__async_assoc_state<_Rp, _Fp>::__execute, __h.get()).detach();
    return future<_Rp>(__h.get());
}

template <class _Rp, class _Fp>
future<_Rp>
__make_mpi_assoc_state(_Fp&& __f)
{
    unique_ptr<__mpi_assoc_state<_Rp, _Fp>, __release_shared_count>
        __h(new __mpi_assoc_state<_Rp, _Fp>(std::forward<_Fp>(__f)));
    // must be an MPI non-blocking operation
    __h.get()->__execute();
    return future<_Rp>(__h.get());
}

template <class _Fp, class... _Args>
class __async_func
{
    tuple<_Fp, _Args...> __f_;

public:
    _LIBCPP_INLINE_VISIBILITY
    explicit __async_func(_Fp&& __f, _Args&&... __args)
        : __f_(std::move(__f), std::move(__args)...) {}

    _LIBCPP_INLINE_VISIBILITY
    __async_func(__async_func&& __f) : __f_(std::move(__f.__f_)) {}

    template <typename ..._ExArgs>
    decltype(auto) operator()(_ExArgs&&... __exargs)
    {
        typedef typename __make_tuple_indices<1+sizeof...(_Args), 1>::type _Index;
        return __execute(_Index(), std::forward<_ExArgs>(__exargs)...);
    }
private:
    template <size_t ..._Indices, typename ..._ExArgs>
    decltype(auto)
    __execute(__tuple_indices<_Indices...>, _ExArgs&&... __exargs)
    {
        return __invoke(std::move(std::get<0>(__f_)),
            std::forward<_ExArgs>(__exargs)...,
            std::move(std::get<_Indices>(__f_))...);
    }
};

inline _LIBCPP_INLINE_VISIBILITY bool __does_policy_contain(launch __policy, launch __value )
{ return (int(__policy) & int(__value)) != 0; }

template <class _Fp, class... _Args>
future<typename __invoke_of<typename decay<_Fp>::type, typename decay<_Args>::type...>::type>
async(launch __policy, _Fp&& __f, _Args&&... __args)
{
    typedef __async_func<typename decay<_Fp>::type, typename decay<_Args>::type...> _BF;
    typedef typename __invoke_of<typename decay<_Fp>::type,
        typename decay<_Args>::type...>::type _Rp;

#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif
        if (__does_policy_contain(__policy, launch::async))
        return __make_async_assoc_state<_Rp>(_BF(__decay_copy(std::forward<_Fp>(__f)),
                                                     __decay_copy(std::forward<_Args>(__args))...));
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch ( ... ) { if (__policy == launch::async) throw ; }
#endif

    if (__does_policy_contain(__policy, launch::deferred))
        return __make_deferred_assoc_state<_Rp>(_BF(__decay_copy(std::forward<_Fp>(__f)),
                                                        __decay_copy(std::forward<_Args>(__args))...));
    return future<_Rp>{};
}

template <class _Fp, class... _Args>
auto
mpi_async(_Fp&& __f, _Args&&... __args)
{
    typedef __async_func<typename decay<_Fp>::type, typename decay<_Args>::type...> _BF;

    return __make_mpi_assoc_state<void>(_BF(__decay_copy(std::forward<_Fp>(__f)),
            __decay_copy(std::forward<_Args>(__args))...));
}

template <class _Rp, class _Fp, class... _Args>
auto
mpi_async(_Fp&& __f, _Args&&... __args)
{
    typedef __async_func<typename decay<_Fp>::type, typename decay<_Args>::type...> _BF;

    return __make_mpi_assoc_state<_Rp>(_BF(__decay_copy(std::forward<_Fp>(__f)),
            __decay_copy(std::forward<_Args>(__args))...));
}

template <class _Fp, class... _Args>
inline _LIBCPP_INLINE_VISIBILITY
future<typename __invoke_of<typename decay<_Fp>::type, typename decay<_Args>::type...>::type>
async(_Fp&& __f, _Args&&... __args)
{
    return async(launch::any, std::forward<_Fp>(__f),
                                    std::forward<_Args>(__args)...);
}

#endif  // _LIBCPP_HAS_NO_VARIADICS

// shared_future

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY shared_future
{
    __assoc_state<_Rp>* __state_;

public:
    _LIBCPP_INLINE_VISIBILITY
    shared_future() _NOEXCEPT : __state_(nullptr) {}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(const shared_future& __rhs) : __state_(__rhs.__state_)
        {if (__state_) __state_->__add_shared();}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future(future<_Rp>&& __f) _NOEXCEPT : __state_(__f.__state_)
        {__f.__state_ = nullptr;}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(shared_future&& __rhs) _NOEXCEPT : __state_(__rhs.__state_)
        {__rhs.__state_ = nullptr;}
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~shared_future();
    shared_future& operator=(const shared_future& __rhs);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future& operator=(shared_future&& __rhs) _NOEXCEPT
        {
            shared_future(std::move(__rhs)).swap(*this);
            return *this;
        }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    // retrieving the value
    _LIBCPP_INLINE_VISIBILITY
    const _Rp& get() const {return __state_->copy();}

    _LIBCPP_INLINE_VISIBILITY
    void swap(shared_future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
shared_future<_Rp>::~shared_future()
{
    if (__state_)
        __state_->__release_shared();
}

template <class _Rp>
shared_future<_Rp>&
shared_future<_Rp>::operator=(const shared_future& __rhs)
{
    if (__rhs.__state_)
        __rhs.__state_->__add_shared();
    if (__state_)
        __state_->__release_shared();
    __state_ = __rhs.__state_;
    return *this;
}

template <class _Rp>
class _LIBCPP_TYPE_VIS_ONLY shared_future<_Rp&>
{
    __assoc_state<_Rp&>* __state_;

public:
    _LIBCPP_INLINE_VISIBILITY
    shared_future() _NOEXCEPT : __state_(nullptr) {}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(const shared_future& __rhs) : __state_(__rhs.__state_)
        {if (__state_) __state_->__add_shared();}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future(future<_Rp&>&& __f) _NOEXCEPT : __state_(__f.__state_)
        {__f.__state_ = nullptr;}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(shared_future&& __rhs) _NOEXCEPT : __state_(__rhs.__state_)
        {__rhs.__state_ = nullptr;}
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~shared_future();
    shared_future& operator=(const shared_future& __rhs);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future& operator=(shared_future&& __rhs) _NOEXCEPT
        {
            shared_future(std::move(__rhs)).swap(*this);
            return *this;
        }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    // retrieving the value
    _LIBCPP_INLINE_VISIBILITY
    _Rp& get() const {return __state_->copy();}

    _LIBCPP_INLINE_VISIBILITY
    void swap(shared_future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
shared_future<_Rp&>::~shared_future()
{
    if (__state_)
        __state_->__release_shared();
}

template <class _Rp>
shared_future<_Rp&>&
shared_future<_Rp&>::operator=(const shared_future& __rhs)
{
    if (__rhs.__state_)
        __rhs.__state_->__add_shared();
    if (__state_)
        __state_->__release_shared();
    __state_ = __rhs.__state_;
    return *this;
}

template <>
class _LIBCPP_TYPE_VIS shared_future<void>
{
    __assoc_sub_state* __state_;

public:
    _LIBCPP_INLINE_VISIBILITY
    shared_future() _NOEXCEPT : __state_(nullptr) {}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(const shared_future& __rhs) : __state_(__rhs.__state_)
        {if (__state_) __state_->__add_shared();}
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future(future<void>&& __f) _NOEXCEPT : __state_(__f.__state_)
        {__f.__state_ = nullptr;}
    _LIBCPP_INLINE_VISIBILITY
    shared_future(shared_future&& __rhs) _NOEXCEPT : __state_(__rhs.__state_)
        {__rhs.__state_ = nullptr;}
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    ~shared_future();
    shared_future& operator=(const shared_future& __rhs);
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    _LIBCPP_INLINE_VISIBILITY
    shared_future& operator=(shared_future&& __rhs) _NOEXCEPT
        {
            shared_future(std::move(__rhs)).swap(*this);
            return *this;
        }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    // retrieving the value
    _LIBCPP_INLINE_VISIBILITY
    void get() const {__state_->copy();}

    _LIBCPP_INLINE_VISIBILITY
    void swap(shared_future& __rhs) _NOEXCEPT {std::swap(__state_, __rhs.__state_);}

    // functions to check state
    _LIBCPP_INLINE_VISIBILITY
    bool valid() const _NOEXCEPT {return __state_ != nullptr;}

    _LIBCPP_INLINE_VISIBILITY
    void wait() const {__state_->wait();}
    template <class _Rep, class _Period>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_for(const chrono::duration<_Rep, _Period>& __rel_time) const
            {return __state_->wait_for(__rel_time);}
    template <class _Clock, class _Duration>
        _LIBCPP_INLINE_VISIBILITY
        future_status
        wait_until(const chrono::time_point<_Clock, _Duration>& __abs_time) const
            {return __state_->wait_until(__abs_time);}
};

template <class _Rp>
inline _LIBCPP_INLINE_VISIBILITY
void
swap(shared_future<_Rp>& __x, shared_future<_Rp>& __y) _NOEXCEPT
{
    __x.swap(__y);
}

template <class _Rp>
inline
shared_future<_Rp>
future<_Rp>::share()
{
    return shared_future<_Rp>(std::move(*this));
}

template <class _Rp>
inline
shared_future<_Rp&>
future<_Rp&>::share()
{
    return shared_future<_Rp&>(std::move(*this));
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

inline
shared_future<void>
future<void>::share()
{
    return shared_future<void>(std::move(*this));
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

}

namespace std
{

template <class _Rp, class _Alloc>
    struct _LIBCPP_TYPE_VIS_ONLY uses_allocator<mpiex::promise<_Rp>, _Alloc>
        : public true_type {};

template <class _Callable, class _Alloc>
struct _LIBCPP_TYPE_VIS_ONLY uses_allocator<mpiex::packaged_task<_Callable>, _Alloc>
    : public true_type {};

}
