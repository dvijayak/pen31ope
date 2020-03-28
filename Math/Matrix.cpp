#include "Matrix.hpp"

Vector3 TransformDirection (Matrix4 const& A, Vector3 const& u)
{
	Vector4 u_homo;
	u_homo[3] = 0;
	for (int i = 0; i < 3; ++i)
		u_homo[i] = u[i];

	Vector4 u_transformed = A * u_homo;

	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = u_transformed[i];
	return result;
}