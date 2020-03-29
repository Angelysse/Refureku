
template <typename T>
void Class::__RFKaddToParents(EAccessSpecifier inheritanceAccess) noexcept
{
	if constexpr (rfk::generated::implements_staticGetArchetype<T, rfk::Class const&()>::value)
	{
		directParents.emplace_back(rfk::Struct::Parent{ inheritanceAccess, &T::staticGetArchetype() });
	}
}