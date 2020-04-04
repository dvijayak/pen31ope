#include "Matrix.hpp"

Vector3 TransformDirection (Matrix4 const& A, Vector3 const& u)
{
	Vector4 u_homo = HomoVector(u, 0.f);

	Vector4 u_transformed = A * u_homo;

	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = u_transformed[i];
	return result;
}