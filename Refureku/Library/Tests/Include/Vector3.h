#pragma once

#include <type_traits>

#include "Refureku/TypeInfo/Archetypes/Class.h"

namespace some_namespace
{
	template <typename T>
	class Vector3
	{
		static_assert(std::is_arithmetic_v<T>, "Can't make vectors with non-arithmetic types.");

		public:
			static inline T exampleStaticField;

			T x;
			T y;
			T z;

			static inline float dot(Vector3 const& lhs, Vector3 const& rhs)
			{
				return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
			}

			inline float sqrSize() const noexcept
			{
				return x * x + y * y + z * z;
			}
	};

	//Define a method which will contain the vector3f metadata
	rfk::Class const& vector3fStaticGetArchetype();
}

using Vector3f = some_namespace::Vector3<float>;