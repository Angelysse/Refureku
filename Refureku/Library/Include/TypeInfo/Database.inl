//
//template <typename T>
//constexpr void Database::fillType(Type& out_type) noexcept
//{
//	TypePart& currPart = out_type.parts.emplace_back(TypePart{ 0u, rfk::ETypePartDescriptor::Undefined, 0u });
//
//	//Const
//	if constexpr (std::is_const_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Const;
//	}
//
//	//Volatile
//	if constexpr (std::is_volatile_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Volatile;
//	}
//
//	if constexpr (std::is_array_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::CArray;
//		currPart.additionalData = static_cast<decltype(currPart.additionalData)>(std::extent_v<T>);
//
//		fillType<std::remove_extent_t<T>>(out_type);
//	}
//	else if constexpr (std::is_pointer_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Ptr;
//		fillType<std::remove_pointer_t<T>>(out_type);
//	}
//	else if constexpr (std::is_lvalue_reference_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::LRef;
//		fillType<std::remove_reference_t<T>>(out_type);
//	}
//	else if constexpr (std::is_rvalue_reference_v<T>)
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::RRef;
//		fillType<std::remove_reference_t<T>>(out_type);
//	}
//	else
//	{
//		currPart.descriptor = currPart.descriptor | ETypePartDescriptor::Value;
//
//		out_type.archetype = Database::getArchetype<std::decay_t<T>>();
//	}
//}
//
//template <typename T>
//Type const& Database::getType() noexcept
//{
//	static rfk::Type	result;
//	static bool			initialized = false;
//
//	if (!initialized)
//	{
//		initialized = true;
//		Database::fillType<T>(result);
//	}
//
//	return result;
//}

