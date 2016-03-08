//===------------------------- future.cpp ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <mpiex/__config.h>

#include <mpiex/future.h>

#include <string>

namespace mpiex
{

void
__assoc_sub_state::__on_zero_shared() _NOEXCEPT
{
    delete this;
}

void
__assoc_sub_state::set_value()
{
    unique_lock<mutex> __lk(__mut_);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__has_value())
        throw future_error(make_error_code(future_errc::promise_already_satisfied));
#endif
    __state_ |= __constructed | ready;
    __cv_.notify_all();
}

void
__assoc_sub_state::set_exception(exception_ptr __p)
{
    unique_lock<mutex> __lk(__mut_);
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__has_value())
        throw future_error(make_error_code(future_errc::promise_already_satisfied));
#endif
    __exception_ = __p;
    __state_ |= ready;
    __cv_.notify_all();
}

void
__assoc_sub_state::__make_ready()
{
    unique_lock<mutex> __lk(__mut_);
    __state_ |= ready;
    __cv_.notify_all();
}

void
__assoc_sub_state::copy()
{
    unique_lock<mutex> __lk(__mut_);
    __sub_wait(__lk);
    if (__exception_ != nullptr)
        rethrow_exception(__exception_);
}

void
__assoc_sub_state::__wait()
{
    unique_lock<mutex> __lk(__mut_);
    __sub_wait(__lk);
}

void
__assoc_sub_state::__sub_wait(unique_lock<mutex>& __lk)
{
    if (!__is_ready())
    {
        if (__state_ & static_cast<unsigned>(deferred))
        {
            __state_ &= ~static_cast<unsigned>(deferred);
            __lk.unlock();
            __execute();
        }
        else
            while (!__is_ready())
                __cv_.wait(__lk);
    }
}

void
__assoc_sub_state::__execute()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    throw future_error(make_error_code(future_errc::no_state));
#endif
}

future<void>::future(__assoc_sub_state* __state)
    : __state_(__state)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__state_->__has_future_attached())
        throw future_error(make_error_code(future_errc::future_already_retrieved));
#endif
    __state_->__add_shared();
    __state_->__set_future_attached();
}

future<void>::~future()
{
    if (__state_)
        __state_->__release_shared();
}

void
future<void>::get()
{
    unique_ptr<__shared_count, __release_shared_count> __(__state_);
    __assoc_sub_state* __s = __state_;
    __state_ = nullptr;
    __s->copy();
}

promise<void>::promise()
    : __state_(new __assoc_sub_state)
{
}

promise<void>::~promise()
{
    if (__state_)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        if (!__state_->__has_value() && __state_->use_count() > 1)
            __state_->set_exception(make_exception_ptr(
                      future_error(make_error_code(future_errc::broken_promise))
                                                      ));
#endif // _LIBCPP_NO_EXCEPTIONS
        __state_->__release_shared();
    }
}

future<void>
promise<void>::get_future()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__state_ == nullptr)
        throw future_error(make_error_code(future_errc::no_state));
#endif
    return future<void>(__state_);
}

void
promise<void>::set_value()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__state_ == nullptr)
        throw future_error(make_error_code(future_errc::no_state));
#endif
    __state_->set_value();
}

void
promise<void>::set_exception(exception_ptr __p)
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    if (__state_ == nullptr)
        throw future_error(make_error_code(future_errc::no_state));
#endif
    __state_->set_exception(__p);
}

shared_future<void>::~shared_future()
{
    if (__state_)
        __state_->__release_shared();
}

shared_future<void>&
shared_future<void>::operator=(const shared_future& __rhs)
{
    if (__rhs.__state_)
        __rhs.__state_->__add_shared();
    if (__state_)
        __state_->__release_shared();
    __state_ = __rhs.__state_;
    return *this;
}

}
