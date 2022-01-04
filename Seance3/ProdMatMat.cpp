#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include "ProdMatMat.hpp"

namespace {
void prodSubBlocks(int iRowBlkA, int iColBlkB, int iColBlkA, int szBlock,
                   const Matrix& A, const Matrix& B, Matrix& C) {

for (int k = iColBlkA; k < std::min(A.nbCols, iColBlkA + szBlock); k++)
{
  //Première parallélisation
  //# pragma omp parallel for default(none) shared(A,B,C, iColBlkB, iColBlkA, iRowBlkA, szBlock,k)  schedule(static)
  for (int j = iColBlkB; j < std::min(B.nbCols, iColBlkB + szBlock); ++j)
  {
      for (int i = iRowBlkA; i < std::min(A.nbRows, iRowBlkA + szBlock); i++)
      {
              C(i, j) += A(i, k) * B(k, j);
      }
  }
}

}
const int szBlock = 1024;
}  // namespace


Matrix operator*(const Matrix& A, const Matrix& B) {
  Matrix C(A.nbRows, B.nbCols, 0.0);
  //prodSubBlocks(0, 0, 0, std::max({A.nbRows, B.nbCols, A.nbCols}), A, B, C);

  //Deuxième parallélisation:
  //#pragma omp parallel for
  for ( int jBlk = 0; jBlk < B.nbCols; jBlk += szBlock )
        {
            for (int iBlk = 0; iBlk < B.nbRows; iBlk += szBlock )
            {
                for ( int kBlk = 0; kBlk < A.nbCols; kBlk += szBlock)
                    prodSubBlocks(iBlk, jBlk, kBlk, szBlock, A, B, C);
            }
        }
  return C;
}