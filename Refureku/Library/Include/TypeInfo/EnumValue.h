#pragma once

#include "TypeInfo/Entity.h"

namespace rfk
{
	class EnumValue : public Entity
	{
		public:
			int64	value = 0;

			EnumValue()												= delete;
			EnumValue(std::string&& name, uint64 id, int64 value)	noexcept;
			EnumValue(EnumValue const&)								= default;
			EnumValue(EnumValue&&)									= default;
			~EnumValue()											= default;

			EnumValue&	operator =(EnumValue const&)	= delete;
			EnumValue&	operator =(EnumValue&&)			= default;
	};
}