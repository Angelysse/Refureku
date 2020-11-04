#include "RefurekuGenerator/Properties/PropertySettingsPropertyRule.h"

#include <cassert>

#include <Kodgen/InfoStructures/MethodInfo.h>
#include <Kodgen/Properties/ComplexProperty.h>

#include "RefurekuGenerator/Properties/NativeProperties.h"
#include "RefurekuGenerator/Properties/PropertyCodeGenData.h"

using namespace rfk;

PropertySettingsPropertyRule::PropertySettingsPropertyRule() noexcept:
	kodgen::DefaultComplexPropertyRule(NativeProperties::propertySettingsProperty, kodgen::EEntityType::Struct | kodgen::EEntityType::Class)
{
}

std::string	PropertySettingsPropertyRule::generateCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, void* userData) const noexcept
{
	PropertyCodeGenData*			data				= reinterpret_cast<PropertyCodeGenData*>(userData);
	kodgen::ComplexProperty	const&	propertySettings	= static_cast<kodgen::ComplexProperty const&>(property);

	if (data->codeGenLocation == rfk::ECodeGenLocation::PropertyAdd)
	{
		return "static_assert(std::is_base_of_v<rfk::Property, " + entity.name + ">, \"[Refureku] Can't attach " + mainPropName + " property to " + entity.getFullName() + " as it doesn't inherit from rfk::Property.\");";
	}
	else if (data->codeGenLocation == rfk::ECodeGenLocation::ClassFooter)
	{
		std::string result;

		//Generate constexpr fields + get overrides
		if (!propertySettings.subProperties.empty())
		{
			//generate targetEntityKind
			result += "public: static constexpr rfk::EEntityKind targetEntityKind = " + propertySettings.subProperties[0] + ";";
			result += "virtual rfk::EEntityKind getTargetEntityKind() const noexcept override { return targetEntityKind; }";

			if (propertySettings.subProperties.size() >= 2)
			{
				//generate shouldInherit
				result += "static constexpr bool shouldInherit = " + propertySettings.subProperties[1] + ";";
				result += "virtual bool getShouldInherit() const noexcept override { return shouldInherit; }";

				if (propertySettings.subProperties.size() >= 3)
				{
					//generate allowMultiple
					result += "static constexpr bool allowMultiple = " + propertySettings.subProperties[2] + ";";
					result += "virtual bool getAllowMultiple() const noexcept override { return allowMultiple; }";
				}
			}
		}

		return result;
	}

	return "";
}