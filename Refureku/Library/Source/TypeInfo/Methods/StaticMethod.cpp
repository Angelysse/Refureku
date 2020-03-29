#include "TypeInfo/Methods/StaticMethod.h"

#include <utility>	//std::forward

using namespace refureku;

StaticMethod::StaticMethod(std::string&&				name,
						   uint64						id,
						   EMethodFlags					flags,
						   std::shared_ptr<ICallable>&&	internalMethod)	noexcept:
	MethodBase(std::forward<std::string>(name), id, flags, std::forward<std::shared_ptr<ICallable>>(internalMethod))
{
}