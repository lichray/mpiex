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

#pragma once

#include <mpi.h>

#include <type_traits>
#include <functional>

namespace mpiex
{

template <MPI_Datatype v>
using mpi_dt_c = std::integral_constant<MPI_Datatype, v>;

template <MPI_Op v>
using mpi_op_c = std::integral_constant<MPI_Op, v>;

template <typename T>
struct mpi_type_of;

template <>
struct mpi_type_of<char> : mpi_dt_c<MPI_CHAR> {};

template <>
struct mpi_type_of<wchar_t> : mpi_dt_c<MPI_WCHAR> {};

template <>
struct mpi_type_of<signed char> : mpi_dt_c<MPI_SIGNED_CHAR> {};

template <>
struct mpi_type_of<short> : mpi_dt_c<MPI_SHORT> {};

template <>
struct mpi_type_of<int> : mpi_dt_c<MPI_INT> {};

template <>
struct mpi_type_of<long> : mpi_dt_c<MPI_LONG> {};

template <>
struct mpi_type_of<long long> : mpi_dt_c<MPI_LONG_LONG> {};

template <>
struct mpi_type_of<float> : mpi_dt_c<MPI_FLOAT> {};

template <>
struct mpi_type_of<double> : mpi_dt_c<MPI_DOUBLE> {};

template <>
struct mpi_type_of<long double> : mpi_dt_c<MPI_LONG_DOUBLE> {};

template <>
struct mpi_type_of<unsigned char> : mpi_dt_c<MPI_UNSIGNED_CHAR> {};

template <>
struct mpi_type_of<unsigned int> : mpi_dt_c<MPI_UNSIGNED> {};

template <>
struct mpi_type_of<unsigned long> : mpi_dt_c<MPI_UNSIGNED_LONG> {};

template <>
struct mpi_type_of<unsigned long long> : mpi_dt_c<MPI_UNSIGNED_LONG_LONG> {};

template <typename T>
struct mpi_op_of;

template <>
struct mpi_op_of<std::plus<>> : mpi_op_c<MPI_SUM> {};

template <>
struct mpi_op_of<std::multiplies<>> : mpi_op_c<MPI_PROD> {};

template <>
struct mpi_op_of<std::logical_and<>> : mpi_op_c<MPI_LAND> {};

template <>
struct mpi_op_of<std::logical_or<>> : mpi_op_c<MPI_LOR> {};

template <>
struct mpi_op_of<std::bit_and<>> : mpi_op_c<MPI_BAND> {};

template <>
struct mpi_op_of<std::bit_or<>> : mpi_op_c<MPI_BOR> {};

template <>
struct mpi_op_of<std::bit_xor<>> : mpi_op_c<MPI_BXOR> {};

}
