# include <iostream>
# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>

// Ecrire dans un fichier est plus pratique 
// pour éviter les entrelacements des messages dans le terminal

int main( int nargs, char* argv[] )
{
    MPI_Init(&nargs, &argv);
    int numero_du_processus, nombre_de_processus;

    MPI_Comm_rank(MPI_COMM_WORLD,
                  &numero_du_processus);
    MPI_Comm_size(MPI_COMM_WORLD, 
                  &nombre_de_processus);

	// Ecriture dans le fichier
    std::stringstream fileName;
	fileName << "Output" << std::setfill('0') << std::setw(5) << numero_du_processus << ".txt";
	std::ofstream output( fileName.str().c_str() );

	output << "Hello world from " 
              << numero_du_processus << " in "
              << nombre_de_processus << " executed" 
              << std::endl;
	output.close();

    MPI_Finalize();
    return EXIT_SUCCESS;
}
