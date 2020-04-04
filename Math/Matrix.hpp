#ifndef Matrix_hpp
#define Matrix_hpp

#include <cassert>
#include <iostream>
#include <type_traits>
#include <array>

#include "Vector.hpp"

/**
 * Generic row-major order matrix class.
 */
template <typename Numeric, uint R, uint C>
class Matrix
{
   static_assert(std::is_arithmetic<Numeric>::value, "Matrices can only contain numeric/arithmetic types, i.e. integers and floating-point numbers");

public:
   typedef Matrix<Numeric, R, C> matrix_type;
   // Row-major order, i.e. the matrix comprises R row vectors of size C
   // Note: knowing this order is critical to correctly implementing the rest of class
   typedef Vector<Numeric, C> row_type;
   typedef Vector<Numeric, R> column_type;
   typedef std::array<row_type, R> elements_type;
   typedef std::array<Numeric, R*C> elements_array_type;

private:
   elements_type m_elements;

public:
   Matrix () : Matrix(0) {}

   /**
    * Construct a matrix from the given array of elements
    * 
    * @param {bool} rowMajor determines how to interpret the order of the elements; if `true`, then the elements are populated row-first, otherwise they are populated column-first; default is row-first 
    */
   explicit Matrix (elements_array_type const& elements, bool rowMajor=true)
   {
      if (rowMajor)
      {
         // R = 3, C = 2
         //   1, 2, 3, 4, 5, 6
         // i 0, 1, 2, 3, 4, 5
         // r 0, 0, 1, 1, 2, 2
         // c 0, 1, 0, 1, 0, 1
         // =>
         // 1 2
         // 3 4
         // 5 6
         for (int i = 0; i < R*C; ++i)
         {
            m_elements[i / C][i % C] = elements[i];
         }
      }
      else // column-major
      {
         // R = 3, C = 2
         //   1, 2, 3, 4, 5, 6
         // i 0, 1, 2, 3, 4, 5
         // r 0, 1, 2, 0, 1, 2
         // c 0, 0, 0, 1, 1, 1
         // =>
         // 1 4
         // 2 5
         // 3 6
         for (int i = 0; i < R*C; ++i)
         {
            m_elements[i % R][i / R] = elements[i];
         }
      }
   }

   /**
    * Allow implicit construction from array of row vectors
    */
   Matrix (elements_type const& rows)
      : m_elements(rows)
   {}

   /**
    * Construct a matrix that is filled with the given value
    */
   explicit Matrix (Numeric const k)
   {
      for (int r = 0; r < R; ++r)
      {
         Fill(m_elements[r], k);
      }
   }

   inline uint Rows () const { return R; }
   inline uint Cols () const { return C; }
   inline uint Columns () const { return C; } // alias
   inline uint M () const { return R; } // alias
   inline uint N () const { return C; } // alias
   
   // TODO: Might be better to change operator[] to return a Vector type, so that we can support double indexing [][] as opposed to overloading operator() for this

   /**
    * Element access; can be used to modify matrix in-place
    */
   inline Numeric & operator[] (uint const index) 		 { return m_elements[index / C][index % C]; }
   inline Numeric	  operator[] (uint const index) const { return m_elements[index / C][index % C]; }
   inline Numeric & operator() (uint const r, uint const c) 		 { return m_elements[r][c]; }
   inline Numeric   operator() (uint const r, uint const c) const { return m_elements[r][c]; }

   /**
    * Row access; can be used to modify matrix in-place
    */
   inline row_type      & operator() (uint const r) 		{ return m_elements[r]; }
   inline row_type const& operator() (uint const r) const { return m_elements[r]; }
   inline row_type      & Row        (uint const r) 		{ return m_elements[r]; }
   inline row_type const& Row        (uint const r) const { return m_elements[r]; }

   /**
    * Column access; cannot modify matrix.
    * Note that this is more expensive than row access as our matrix storage is row-major order
    */
   inline column_type Col (uint const c) const
   {
      column_type result;
      for (int r = 0; r < R; ++r)
      {
         result[r] = m_elements[r][c];
      }
      return result;
   }

   static matrix_type Identity ()
   {
      Matrix<Numeric, R, C> I;
      for (int r = 0; r < R; ++r)
      {
         for (int c = 0; c < C; ++c)
         {
            I(r, c) = r == c ? static_cast<Numeric>(1) : static_cast<Numeric>(0);
         }
      }
      return I;
   }
};

//// Typedefs ////

typedef Matrix<float, 3, 1> ColMatrix3;
typedef Matrix<float, 1, 3> RowMatrix3;
typedef Matrix<float, 2, 1> ColMatrix2;
typedef Matrix<float, 1, 2> RowMatrix2;
typedef Matrix<float, 4, 1> ColMatrix4;
typedef Matrix<float, 1, 4> RowMatrix4;
typedef Matrix<float, 2, 2> Matrix2;
typedef Matrix<float, 3, 3> Matrix3;
typedef Matrix<float, 4, 4> Matrix4;

typedef Matrix<int, 3, 1> ColMatrix3Int;
typedef Matrix<int, 1, 3> RowMatrix3Int;
typedef Matrix<int, 2, 1> ColMatrix2Int;
typedef Matrix<int, 1, 2> RowMatrix2Int;
typedef Matrix<int, 2, 2> Matrix2Int;
typedef Matrix<int, 3, 3> Matrix3Int;
typedef Matrix<int, 4, 4> Matrix4Int;

typedef Matrix<uint, 3, 1> ColMatrix3UInt;
typedef Matrix<uint, 1, 3> RowMatrix3UInt;
typedef Matrix<uint, 2, 1> ColMatrix2UInt;
typedef Matrix<uint, 1, 2> RowMatrix2UInt;
typedef Matrix<uint, 2, 2> Matrix2UInt;
typedef Matrix<uint, 3, 3> Matrix3UInt;
typedef Matrix<uint, 4, 4> Matrix4UInt;

typedef Matrix<uint8_t, 3, 1> ColMatrix3Byte;
typedef Matrix<uint8_t, 1, 3> RowMatrix3Byte;
typedef Matrix<uint8_t, 2, 1> ColMatrix2Byte;
typedef Matrix<uint8_t, 1, 2> RowMatrix2Byte;
typedef Matrix<uint8_t, 2, 2> Matrix2Byte;
typedef Matrix<uint8_t, 3, 3> Matrix3Byte;
typedef Matrix<uint8_t, 4, 4> Matrix4Byte;

template <typename Numeric, uint R, uint C>
std::ostream& operator<< (std::ostream& os, const Matrix<Numeric, R, C>& A)
{
   os << "[";
      
   for (int r = 0; r < R; ++r)
   {
      for (int c = 0; c < C; ++c)
      {
         os << A(r, c);
         if (c < C-1)
            os << "  ";
      }

      if (r < R-1)
         os << std::endl << " ";
   }
   os << "]";

   return os;
}

//// Convenience constructors, as free functions ////

template <typename Numeric, uint N>
Matrix<Numeric, N, N> MakeMatrix (Matrix<Numeric, N-1, N-1> A)
{
   Matrix<Numeric, N, N> result(Matrix<Numeric, N, N>::Identity());
   for (int r = 0; r < N-1; ++r)
   {
      for (int c = 0; c < N-1; ++c)
      {
         result(r,c) = A(r,c);
      }
   }
}

//// Operations, as free functions ////

/**
 * Transpose
 */
template <typename Numeric, uint R, uint C>
Matrix<Numeric, C, R> operator~ (Matrix<Numeric, R, C> const& A)
{
   Matrix<Numeric, C, R> result;
   for (int r = 0; r < R; ++r)
   {
      for (int c = 0; c < C; ++c)
      {
         result(c, r) = A(r, c);
      }
   }
   return result;
}

/**
 * Scale by -1
 */
template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> operator- (Matrix<Numeric, R, C> const& A)
{
   return A * static_cast<Numeric>(-1);
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> operator+ (Matrix<Numeric, R, C> const& A, Matrix<Numeric, R, C> const& B)
{
   Matrix<Numeric, R, C> result;
   for (int i = 0; i < R*C; ++i)
      result[i] = A[i] + B[i];
   return result;
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> & operator+= (Matrix<Numeric, R, C> & A, Matrix<Numeric, R, C> const& B)
{
   for (int i = 0; i < R*C; ++i)
      A[i] += B[i];
   return A;
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> operator- (Matrix<Numeric, R, C> const& A, Matrix<Numeric, R, C> const& B)
{
   Matrix<Numeric, R, C> result;
   for (int i = 0; i < R*C; ++i)
      result[i] = A[i] - B[i];
   return result;
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> & operator-= (Matrix<Numeric, R, C> & A, Matrix<Numeric, R, C> const& B)
{
   for (int i = 0; i < R*C; ++i)
      A[i] -= B[i];
   return A;
}

/**
 * Scalar product
 */
template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> operator* (Matrix<Numeric, R, C> const& A, float const k)
{
   Matrix<Numeric, R, C> result;
   for (int i = 0; i < R*C; ++i)
      result[i] = static_cast<Numeric>(A[i] * k);
   return result;
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> & operator*= (Matrix<Numeric, R, C> & A, float const k)
{
   for (int i = 0; i < R*C; ++i)
      A[i] = static_cast<Numeric>(A[i] * k);
   return A;
}

/**
 * Scalar division, i.e. inverse of scalar product
 */
template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> operator/ (Matrix<Numeric, R, C> const& A, float const k)
{
   Matrix<Numeric, R, C> result;
   for (int i = 0; i < R*C; ++i)
      result[i] = static_cast<Numeric>(A[i] / k);
   return result;
}

template <typename Numeric, uint R, uint C>
Matrix<Numeric, R, C> & operator/= (Matrix<Numeric, R, C> & A, float const k)
{
   for (int i = 0; i < R*C; ++i)
      A[i] = static_cast<Numeric>(A[i] / k);
   return A;
}

/**
 * Ye ol' non-commutative multiplication between matrices
 */
template <typename Numeric, uint M, uint N, uint O, uint P>
Matrix<Numeric, M, P> operator* (Matrix<Numeric, M, N> const& A, Matrix<Numeric, O, P> const& B)
{
   // We use the beauty of template specialization to implement two matrix multiplication properties at compile-time:
   // 1) Two matrices A and B can only be multipled iff the number of columns of A is the same as the number of rows of B
   static_assert(N == O, "Multiplication of matrix A by matrix B is not defined when columns(A) != rows(B)");
   // 2) The resultant matrix of the above operation is of dimensions rows(A) x columns(B)

   Matrix<Numeric, M, P> result;
   for (int r = 0; r < M; ++r)
      for (int c = 0; c < P; ++c)
         for (uint i = 0; i < N; ++i)
            result(r,c) += A(r,i) * B(i,c);
   return result;
}

template <typename Numeric, uint N>
Matrix<Numeric, N, N> & operator*= (Matrix<Numeric, N, N> & A, Matrix<Numeric, N, N> const& B)
{
   for (int r = 0; r < N; ++r)
   {
      for (int c = 0; c < N; ++c)
      {
         float result = 0;
         for (int i = 0; i < N; ++i)
            result += A(r,i) * B(i,c);
         A(r,c) = result;
      }
   }
   return A;
}

/**
 * Matrix-Vector multiplication. Always assumes a column vector (i.e. Nx1 matrix), for simplicity.
 */
template <typename Numeric, uint M, uint N>
Vector<Numeric, M> operator* (Matrix<Numeric, M, N> const& A, Vector<Numeric, N> const& v)
{
   Vector<Numeric, M> result;
   for (int i = 0; i < M; ++i)
      for (int j = 0; j < N; ++j)
         result[i] += A(i,j) * v[j];
   return result;
}

/**
 * Convenience: Homogeneous coordinates transformation (multiply a square MxM matrix by a M-1 vector that is turned into a M vector)
 */
template <typename Numeric, uint M>
Vector<Numeric, M-1> operator* (Matrix<Numeric, M, M> const& A, Vector<Numeric, M-1> const& v)
{
   // Add `1` as the Mth element of the vector
   Vector<Numeric, M> v_homo = HomoVector(v);

   // Do the transform
   Vector<Numeric, M> transformed = A * v_homo;

   // Project back into M-1 dimension
   return ProjectToHyperspace(transformed);
}

template <typename Numeric>
float Determinant (Matrix<Numeric, 1, 1> const& A)
{
   return A[0];
}

template <typename Numeric>
float Determinant (Matrix<Numeric, 2, 2> const& A)
{
   return A(0,0) * A(1,1) - A(1,0) * A(0,1);
}

template <typename Numeric>
float Determinant (Matrix<Numeric, 3, 3> const& A)
{
   // Indices:
   // 0,0 0,1 0,2
   // 1,0 1,1 1,2
   // 2,0 2,1 2,2
   return A(0,0) * ( A(1,1) * A(2,2) - A(2,1) * A(1,2) )
        - A(0,1) * ( A(1,0) * A(2,2) - A(2,0) * A(1,2) )
        + A(0,2) * ( A(1,0) * A(2,1) - A(2,0) * A(1,1) )
        ;
}

template <typename Numeric>
float Determinant (Matrix<Numeric, 4, 4> const& A)
{
   // Indices:
   // 0,0 0,1 0,2 0,3
   // 1,0 1,1 1,2 1,3
   // 2,0 2,1 2,2 2,3
   // 3,0 3,1 3,2 3,3
   
   // I already know how to compute the determinant using cofactor expansion,
   // so I chose expediency by copying this hardcoded solution from https://stackoverflow.com/a/2937973/3477043
   return
         A(0,3) * A(1,2) * A(2,1) * A(3,0) - A(0,2) * A(1,3) * A(2,1) * A(3,0) -
         A(0,3) * A(1,1) * A(2,2) * A(3,0) + A(0,1) * A(1,3) * A(2,2) * A(3,0) +
         A(0,2) * A(1,1) * A(2,3) * A(3,0) - A(0,1) * A(1,2) * A(2,3) * A(3,0) -
         A(0,3) * A(1,2) * A(2,0) * A(3,1) + A(0,2) * A(1,3) * A(2,0) * A(3,1) +
         A(0,3) * A(1,0) * A(2,2) * A(3,1) - A(0,0) * A(1,3) * A(2,2) * A(3,1) -
         A(0,2) * A(1,0) * A(2,3) * A(3,1) + A(0,0) * A(1,2) * A(2,3) * A(3,1) +
         A(0,3) * A(1,1) * A(2,0) * A(3,2) - A(0,1) * A(1,3) * A(2,0) * A(3,2) -
         A(0,3) * A(1,0) * A(2,1) * A(3,2) + A(0,0) * A(1,3) * A(2,1) * A(3,2) +
         A(0,1) * A(1,0) * A(2,3) * A(3,2) - A(0,0) * A(1,1) * A(2,3) * A(3,2) -
         A(0,2) * A(1,1) * A(2,0) * A(3,3) + A(0,1) * A(1,2) * A(2,0) * A(3,3) +
         A(0,2) * A(1,0) * A(2,1) * A(3,3) - A(0,0) * A(1,2) * A(2,1) * A(3,3) -
         A(0,1) * A(1,0) * A(2,2) * A(3,3) + A(0,0) * A(1,1) * A(2,2) * A(3,3);
}

/**
 * Calculate square matrix inverse assuming it is orthogonal. The inverse of an orthogonal matrix is simply its transpose!
 */
template <typename Numeric, uint N>
Matrix<Numeric, N, N> Inverse_Orthogonal (Matrix<Numeric, N, N> const& A)
{
   return ~A;
}

/**
 * Calculate inverse of 4x4 homogenous transformation assuming it contains just a rotation and a translation.
 * 
 * We use the same trick as with change-of-basis matrix calculations, namely M^-1 = (T * R)^-1 = R^-1 * T^-1
 * \see Camera::LookAt
 */
template <typename Numeric>
Matrix<Numeric, 4, 4> Inverse_RotationTranslation (Matrix<Numeric, 4, 4> const& A)
{	
   typedef Matrix<Numeric, 4 , 4> matrix_type;

   matrix_type rotation(typename matrix_type::elements_array_type({
      A(0,0), A(0,1), A(0,2), 0,
      A(1,0), A(1,1), A(1,2), 0,
      A(2,0), A(2,1), A(2,2), 0,
           0,		 0, 		0, 1
   }));	

   matrix_type inverseOfTranslation(typename matrix_type::elements_array_type({
      1, 0, 0, -A(0,3),
      0, 1, 0, -A(1,3),
      0, 0, 1, -A(2,3),
      0, 0, 0,      1
   }));

   return Inverse_Orthogonal(rotation) * inverseOfTranslation;
}

//// Super specialized operations ////

Vector3 TransformDirection (Matrix4 const& A, Vector3 const& u);

#endif