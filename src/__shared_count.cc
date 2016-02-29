#include <mpiex/__shared_count.h>

namespace mpiex
{

__shared_count::~__shared_count()
{
}

void
__shared_count::__add_shared() _NOEXCEPT
{
    ++__shared_owners_;
}

bool
__shared_count::__release_shared() _NOEXCEPT
{
    if (--__shared_owners_ == -1)
    {
        __on_zero_shared();
        return true;
    }
    return false;
}

}
