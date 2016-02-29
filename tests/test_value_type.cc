#include "mpi_catch.h"

#include <mpiex.h>

TEST_CASE("basic")
{
	auto f = mpiex::async([] { return 3; });
	REQUIRE(f.get() == 3);
}
