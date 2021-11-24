// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>

#include <algorithm>

#include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
public:
    Matrix (int dim);
    Matrix( int nrows, int ncols );
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
    assert( u.size() == unsigned(m_ncols) );
    std::vector<double> v(m_nrows, 0.);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v[i] += A(i,j)*u[j];
        }            
    }
    return v;
}

//Paralléliser les colonnes
std::vector<double> 
Matrix::columnProduct(const std::vector<double>& u, int index_dep, int index_end) const
{
    std::vector<double> v(m_nrows, 0.);

    const Matrix& A = *this;
    assert( u.size() == unsigned(m_ncols) );

    for(int i=0;i<m_nrows;++i)
    {
        for(int j=index_dep;j<index_end;++j)
        {
            v[i]+=A(i,j)*u[j];
        }
    }
    return v;
}

//Paralléliser les lignes
std::vector<double> 
Matrix::lineProduct(const std::vector<double>& u, int index_dep, int index_end) const
{
    std::vector<double> v(m_nrows, 0.);

    const Matrix& A = *this;
    assert( u.size() == unsigned(m_ncols) );

    for(int i=index_dep;i<index_end;++i)
    {
        for(int j=0;j<m_ncols;++j)
        {
            v[i]+=A(i,j)*u[j];
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
// =====================================================================
int main( int nargs, char* argv[] )
{
    const int N = 120;
    Matrix A(N);
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    
    //CODE SEQUENTIEL
    //std::cout  << "A : " << A << std::endl;
    //std::cout << " u : " << u << std::endl;
    //std::vector<double> v = A*u;
    //std::cout << "A.u = " << v << std::endl;
    

    MPI_Init( &nargs, &argv );
    // Pour des raison préfère toujours cloner le communicateur global
    // MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    // On interroge le communicateur global pour connaître le nombre de processus
    // qui ont été lancés par l'utilisateur :
    int nbp;
    MPI_Comm_size(globComm, &nbp);
    // On interroge le communicateur global pour connaître l'identifiant qui
    // m'a été attribué ( en tant que processus ). Cet identifiant est compris
    // entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
    // l'utilisateur )
    int rank;
    MPI_Comm_rank(globComm, &rank);

    std::vector<double> final_result;

    int index=(A.getRowNumber())/nbp;

    std::vector<double> res={0};
    res=A.columnProduct(u,rank*index,(rank+1)*index); // On peut le faire par lignes aussi en remplaçant par lineProduct

    final_result.resize(res.size());

    std::cout << final_result.size() << std::endl;

    MPI_Allreduce(res.data(),final_result.data(),res.size(),MPI_DOUBLE,MPI_SUM,globComm);
    // A la fin du programme, on doit synchroniser une dernière fois tous les processus
    // afin qu'aucun processus ne se termine pendant que d'autres processus continue à
    // tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
    // qui ne seront pas encore terminés.


    MPI_Finalize();
    std::cout << "Resultat final: "<< final_result <<std::endl;


    return EXIT_SUCCESS;

}