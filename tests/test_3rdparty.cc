#include "mpi_catch.h"

TEST_CASE("MPI main")
{
	int rnk;
	MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
	CAPTURE(rnk);
}
