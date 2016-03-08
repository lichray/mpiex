#include "mpi_catch.h"

#include <mpiex.h>

#include <cmath>
#include <iostream>

int reduce_sum(mpiex::communicator comm, float a) {
  auto p = comm.size();
  auto b = float(p);
  auto rank = comm.rank();

  int parent = int((rank-1)/2);
  int left = rank*2+1;
  int right = rank*2+2;

  if(left<p) {
    auto fu = recv(comm, left, b);
    fu.wait();
    a = a + b;
  }
  if(right<p) {
    auto fu = mpiex::receive<float>(comm, right);
    b = fu.get();
    a = a + b;
  }
  if(rank>0) {
    auto fu = send(comm, parent, a);
    // wait upon destruction
  }
  return a;
}

TEST_CASE("example2")
{
  auto comm = mpiex::communicator();
  float a = std::pow(comm.rank() + 1, 2);

  a = reduce_sum(comm, a);

  if (comm.rank() == 0)
      REQUIRE(a == 55);
}
