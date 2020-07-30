#include "GeneratedEntityCodeTemplate.h"

using namespace rfk;

std::hash<std::string> const GeneratedEntityCodeTemplate::stringHasher;

void GeneratedEntityCodeTemplate::generateCode(kodgen::GeneratedFile& /* generatedFile */, kodgen::EntityInfo const& entityInfo) noexcept
{
	_currentEntityId = std::to_string(stringHasher(entityInfo.id)) + "u";
}

std::string	GeneratedEntityCodeTemplate::fillEntityProperties(kodgen::EntityInfo const& info, std::string const& entityVarName) const noexcept
{
	std::string result;

	for (kodgen::SimpleProperty const& prop : info.properties.simpleProperties)
	{
		result += entityVarName + "properties.simpleProperties.emplace(\"" + prop.mainProperty + "\"); ";
	}

	for (kodgen::ComplexProperty const& prop : info.properties.complexProperties)
	{
		for (std::string subProp : prop.subProperties)
		{
			result += entityVarName + "properties.complexProperties.emplace(\"" + prop.mainProperty + "\", \"" + subProp + "\"); ";
		}
	}

	return result;
}