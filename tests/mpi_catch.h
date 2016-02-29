#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <mpi.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int r = Catch::Session().run(argc, argv);
	MPI_Finalize();

	return r;
}
