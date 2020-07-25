#include "TypeInfo/Namespace.h"

using namespace rfk;

Namespace::Namespace(std::string&& newName, uint64 newId) noexcept:
	Entity(std::forward<std::string>(newName), newId, Entity::EKind::Namespace)
{
	std::cout << "Namespace::Namespace() : " << name << std::endl;
}