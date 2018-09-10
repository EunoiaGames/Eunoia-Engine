#include "Math.h"

namespace Eunoia { namespace Math {

	Vector3f Vector3f::Rotate(const Quaternion& quat) const
	{
		Quaternion conjugate = quat.GetConjugate();
		Quaternion w = quat * (*this) * conjugate;
		return Vector3f(w.GetX(), w.GetY(), w.GetZ());
	}

} }