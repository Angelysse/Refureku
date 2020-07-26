#include "GeneratedEnumCodeTemplate.h"

#include <cassert>

using namespace rfk;

void GeneratedEnumCodeTemplate::generateCode(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo const& entityInfo) noexcept
{
	GeneratedEntityCodeTemplate::generateCode(generatedFile, entityInfo);

	assert(entityInfo.entityType == kodgen::EntityInfo::EType::Enum);

	generateEnumCode(generatedFile, static_cast<kodgen::EnumInfo const&>(entityInfo));
}

void GeneratedEnumCodeTemplate::generateEnumCode(kodgen::GeneratedFile& generatedFile, kodgen::EnumInfo const& enumInfo) const noexcept
{
	std::string	mainMacroName			= internalPrefix + getCurrentEntityId() + "_GENERATED";
	std::string specializationMacroName	= generateGetEnumSpecialization(generatedFile, enumInfo);
	std::string registerMacroName		= generateRegistrationMacro(generatedFile, enumInfo);

	generatedFile.writeMacro(std::move(mainMacroName),
							 "namespace rfk {",
								std::move(specializationMacroName),
								std::move(registerMacroName),
							 "}");
}

std::string GeneratedEnumCodeTemplate::generateGetEnumSpecialization(kodgen::GeneratedFile& generatedFile, kodgen::EnumInfo const& enumInfo) const noexcept
{
	std::string macroName	= internalPrefix + getCurrentEntityId() + "_GenerateGetEnumSpecialization";
	std::string typeName	= enumInfo.type.getCanonicalName();
	std::string properties;

	generatedFile.writeLines("#define " + macroName + "\t\\",
							 "	template <>\t\\",
							 "	inline rfk::Enum const* getEnum<" + typeName + ">() noexcept\t\\",
							 "	{\t\\",
							 "		static bool			initialized = false;\t\\",
							 "		static rfk::Enum	type(\"" + enumInfo.name + "\", " + std::to_string(stringHasher(enumInfo.id)) + "u, sizeof(" + typeName + "));\t\\");
	
	generatedFile.writeLines("		if (!initialized)\t\\",
							 "		{\t\\",
							 "			initialized = true;\t\\");

	//Fill enum properties
	properties = fillEntityProperties(enumInfo, "type.");
	if (!properties.empty())
	{
		generatedFile.writeLine("			" + properties + "\t\\");
	}

	if (!enumInfo.enumValues.empty())
	{
		generatedFile.writeLines("			std::unordered_set<EnumValue, Entity::NameHasher, Entity::EqualName>::iterator	valueIt;\t\\",
								 "			[[maybe_unused]] rfk::EnumValue*												ev;\t\\",
								 "			type.values.reserve(" + std::to_string(enumInfo.enumValues.size()) + ");\t\\");

		for (kodgen::EnumValueInfo const& ev : enumInfo.enumValues)
		{
			generatedFile.writeLine("			valueIt = type.values.emplace(\"" + ev.name + "\", " + std::to_string(stringHasher(ev.id)) + "u, " + std::to_string(ev.defaultValue) + "u).first;\t\\");

			//Fill enum value properties
			properties = fillEntityProperties(ev, "ev->");
			if (!properties.empty())
			{
				generatedFile.writeLines("			ev = const_cast<rfk::EnumValue*>(&*valueIt);\t\\",
										 "			" + properties + "\t\\");
			}
		}
	}

	generatedFile.writeLine("		}\t\\");

	generatedFile.writeLines("		return &type;\t\\",
							 "	}",
							 "");

	return macroName;
}

std::string GeneratedEnumCodeTemplate::generateRegistrationMacro(kodgen::GeneratedFile& generatedFile, kodgen::EnumInfo const& enumInfo) const noexcept
{
	//Don't register to database if the class is contained in another entity (namespace / class)
	if (enumInfo.outerEntity != nullptr)
	{
		return std::string();
	}
	else
	{
		std::string macroName = internalPrefix + getCurrentEntityId() + "_RegisterEnum";

		//Wrap into a generated namespace to avoid pollution in rfk namespace
		generatedFile.writeMacro(std::string(macroName),
								 "namespace generated { inline rfk::ArchetypeRegisterer registerer" + getCurrentEntityId() + " = rfk::getEnum<" + enumInfo.type.getCanonicalName() + ">(); }");

		return macroName;
	}
}