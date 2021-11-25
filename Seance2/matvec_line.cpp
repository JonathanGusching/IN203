// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>

# include <chrono>
#include <algorithm>

#include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
public:
    Matrix (int dim);
    Matrix( int nrows, int ncols );
    Matrix( int nrows, int ncols, int j_start );

    Matrix( const Matrix& A ) = delete;
    Matrix( Matrix&& A ) = default;
    ~Matrix() = default;

    Matrix& operator = ( const Matrix& A ) = delete;
    Matrix& operator = ( Matrix&& A ) = default;

    int getRowNumber(){ return m_nrows; }
    
    double& operator () ( int i, int j ) {
        return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () ( int i, int j ) const {
        return m_arr_coefs[i + j*m_nrows];
    }
    
    std::vector<double> operator * ( const std::vector<double>& u ) const;
    std::vector<double> columnProduct(const std::vector<double>& u, int index_dep, int index_end) const;
    std::vector<double> lineProduct(const std::vector<double>& u, int index_dep, int index_end) const;
    std::ostream& print( std::ostream& out ) const
    {
        const Matrix& A = *this;
        out << "[\n";
        for ( int i = 0; i < m_nrows; ++i ) {
            out << " [ ";
            for ( int j = 0; j < m_ncols; ++j ) {
                out << A(i,j) << " ";
            }
            out << " ]\n";
        }
        out << "]";
        return out;
    }
private:
    int m_nrows, m_ncols;
    int offset=0;
    std::vector<double> m_arr_coefs;
};
// ---------------------------------------------------------------------
inline std::ostream& 
operator << ( std::ostream& out, const Matrix& A )
{
    return A.print(out);
}
// ---------------------------------------------------------------------
inline std::ostream&
operator << ( std::ostream& out, const std::vector<double>& u )
{
    out << "[ ";
    for ( const auto& x : u )
        out << x << " ";
    out << " ]";
    return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * ( const std::vector<double>& u ) const
{
    const Matrix& A = *this;
    //assert( u.size() == unsigned(m_ncols) );
    std::vector<double> v(m_nrows, 0.);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v[i] += A(i,j)*u[offset+j];
        }            
    }
    return v;
}
// =====================================================================
Matrix::Matrix (int dim) : m_nrows(dim), m_ncols(dim),
                           m_arr_coefs(dim*dim)
{
    for ( int i = 0; i < dim; ++ i ) {
        for ( int j = 0; j < dim; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
    }
}
// ---------------------------------------------------------------------
Matrix::Matrix( int nrows, int ncols ) : m_nrows(nrows), m_ncols(ncols),
                                         m_arr_coefs(nrows*ncols)
{
    int dim = (nrows > ncols ? nrows : ncols );
    for ( int i = 0; i < nrows; ++ i ) {
        for ( int j = 0; j < ncols; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
    }    
}
// ---------------------------------------------------------------------
Matrix::Matrix( int nrows, int ncols, int j_start ) : m_nrows(nrows), m_ncols(ncols+j_start), offset(j_start),
                                         m_arr_coefs(nrows*(ncols+j_start))
{
    int dim = (nrows > ncols ? nrows : ncols );
    for ( int i = 0; i < nrows; ++ i ) {
        /* Ou bien on remplit les cases inutiles par des zéros pour avoir le décalage
        for ( int j = 0; j < j_start; ++j ) {
            (*this)(i,j) = 0; 
        }
        for ( int j = j_start; j < j_start+ncols; ++j ) {
            (*this)(i,j) = (i+j)%dim;
        }
        */

        //Ou bien on rajoute un champ à notre classe (moins couteux en mémoire) cf champ privé "offset" et on modifie le produit et la classe
        for ( int j = 0; j < ncols; ++j ) {
            (*this)(i,j) = (i+j_start+j)%dim;
        }
    }    
}
// =====================================================================
int main( int nargs, char* argv[] )
{
    const int N = 120;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    

    MPI_Init( &nargs, &argv );
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);

    int nbp;
    MPI_Comm_size(globComm, &nbp);

    int rank;
    MPI_Comm_rank(globComm, &rank);

    std::vector<double> final_result;


    int nb_col=N/nbp;   

    Matrix A(N,nb_col,rank*nb_col);

    std::vector<double> res={0.};
    res=A*u;

    final_result.resize(N);

    MPI_Allreduce(res.data(),final_result.data(),res.size(),MPI_DOUBLE,MPI_SUM,globComm);


    // pour n'avoir qu'un seul affichage
    if(rank==0)
        std::cout << "Resultat final: "<< final_result <<std::endl;
    MPI_Finalize();


    return EXIT_SUCCESS;

}