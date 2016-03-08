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

namespace mpiex
{

static_assert(std::is_same<MPI_Datatype, int>{}, "");

template <int v>
using int_c = std::integral_constant<int, v>;

template <typename T>
struct mpi_type_of;

template <>
struct mpi_type_of<char> : int_c<MPI_CHAR> {};

template <>
struct mpi_type_of<wchar_t> : int_c<MPI_WCHAR> {};

template <>
struct mpi_type_of<signed char> : int_c<MPI_SIGNED_CHAR> {};

template <>
struct mpi_type_of<short> : int_c<MPI_SHORT> {};

template <>
struct mpi_type_of<int> : int_c<MPI_INT> {};

template <>
struct mpi_type_of<long> : int_c<MPI_LONG> {};

template <>
struct mpi_type_of<long long> : int_c<MPI_LONG_LONG> {};

template <>
struct mpi_type_of<float> : int_c<MPI_FLOAT> {};

template <>
struct mpi_type_of<double> : int_c<MPI_DOUBLE> {};

template <>
struct mpi_type_of<long double> : int_c<MPI_LONG_DOUBLE> {};

template <>
struct mpi_type_of<unsigned char> : int_c<MPI_UNSIGNED_CHAR> {};

template <>
struct mpi_type_of<unsigned int> : int_c<MPI_UNSIGNED> {};

template <>
struct mpi_type_of<unsigned long> : int_c<MPI_UNSIGNED_LONG> {};

template <>
struct mpi_type_of<unsigned long long> : int_c<MPI_UNSIGNED_LONG_LONG> {};

}
