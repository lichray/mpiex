/*-
 * Copyright (c) 2016 Zhihao Yuan.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "future.h"
#include "traits.h"
#include "communicator.h"

namespace mpiex
{

template <typename T>
inline
future<void>
send(communicator comm, int dest, T const* p, size_t sz, int tag = 0)
{
    return mpi_async([=]
        {
            MPI_Request r;
            MPI_Isend(p, int(sz), mpi_type_of<T>{}, dest, tag, comm.get(), &r);
            return r;
        });
}

template <typename T>
inline
future<void>
recv(communicator comm, int src, T* p, size_t sz, int tag = 0)
{
    return mpi_async([=]
        {
            MPI_Request r;
            MPI_Irecv(p, int(sz), mpi_type_of<T>{}, src, tag, comm.get(), &r);
            return r;
        });
}

template <typename T>
inline
future<void>
send(communicator comm, int dest, T const& v, int tag = 0)
{
    return send(comm, dest, std::addressof(v), 1, tag);
}

template <typename T>
inline
future<void>
recv(communicator comm, int src, T& v, int tag = 0)
{
    return recv(comm, src, std::addressof(v), 1, tag);
}

}
