/*
file: vector.h
*/

#pragma once

namespace M {

	/* Vector2 */

	struct Vector2 {
		union {
			struct {
				float x, y;
			};
			float vec[2];
		};

		Vector2(void) { }
		Vector2(const Vector2& other) : x(other.x), y(other.y) { }
		Vector2(float x, float y) : x(x), y(y) { }

		Vector2& operator=(const Vector2& other) {
			if(&other == this) return *this;
			x = other.x;
			y = other.y;
			return *this;
		}
	};

	/* Vector3 */

	struct Vector3 {
		union {
			struct {
				float x, y, z;
			};
			float vec[3];
		};

		Vector3(void) { }
		Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

		Vector3& operator=(const Vector3& other) {
			if(&other == this) return *this;
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3 operator-(void) const {
			return Vector3(-x, -y, -z);
		}

		Vector3& operator+=(const Vector3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		void Normalize(void) {
			const float len = Length();
			x /= len;
			y /= len;
			z /= len;
		}

		float Length(void) const {
			return sqrt(x * x + y * y + z * z);
		}

		static Vector3 Zero;
	};

	static std::ostream& operator<<(std::ostream& stream, const M::Vector3& vector) {
		stream << "("
			<< vector.x << ", "
			<< vector.y << ", "
			<< vector.z
			<< ")";
		return stream;
	}

	inline Vector3 operator+(const Vector3& lhp, const Vector3& rhp) {
		return Vector3(lhp.x + rhp.x, lhp.y + rhp.y, lhp.z + rhp.z);
	}

	inline Vector3 operator-(const Vector3& lhp, const Vector3& rhp) {
		return Vector3(lhp.x - rhp.x, lhp.y - rhp.y, lhp.z - rhp.z);
	}

	inline Vector3 operator*(float scalar, const Vector3& vector) {
		return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z);
	}

	inline Vector3 operator*(const Vector3& vector, float scalar) {
		return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z);
	}

	inline Vector3 operator/(const Vector3& vector, float scalar) {
		return Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
	}

	inline Vector3 Negate(const Vector3& vec) {
		return Vector3(-vec.x, -vec.y, -vec.z);
	}

	inline float Dot(const Vector3& lhp, const Vector3& rhp) {
		return lhp.x * rhp.x + lhp.y * rhp.y + lhp.z * rhp.z;
	}

	inline Vector3 Cross(const Vector3& lhp, const Vector3& rhp) {
		return Vector3(lhp.y * rhp.z - lhp.z * rhp.y,
					   lhp.z * rhp.x - lhp.x * rhp.z,
					   lhp.x * rhp.y - lhp.y * rhp.x);
	}

	inline Vector3 Normalize(const Vector3& vector) {
		float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		return vector / length;
	}

	inline float Length(const Vector3& vector) {
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	static float Distance(const Vector3& lhp, const Vector3& rhp) {
		return M::Length(rhp - lhp);
	}

	static bool LinearlyDependent(const Vector3& u, const Vector3& v) {
		return COM::AlmostEqual(u.x * v.y - u.y * v.x, 0.0f) &&
			   COM::AlmostEqual(u.x * v.z - u.z * v.x, 0.0f);
	}

} // namespace M