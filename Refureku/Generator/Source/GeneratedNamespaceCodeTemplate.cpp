#include "RefurekuGenerator/GeneratedNamespaceCodeTemplate.h"

#include <cassert>

#include <Kodgen/Parsing/FileParser.h>	//For FileParser::parsingMacro

#include "RefurekuGenerator/GeneratedVariableCodeTemplate.h"
#include "RefurekuGenerator/GeneratedFunctionCodeTemplate.h"
#include "RefurekuGenerator/Helpers.h"

using namespace rfk;

void GeneratedNamespaceCodeTemplate::generateCode(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo const& entityInfo) noexcept
{
	GeneratedEntityCodeTemplate::generateCode(generatedFile, entityInfo);

	assert(entityInfo.entityType == kodgen::EEntityType::Namespace);

	kodgen::NamespaceInfo const& namespaceInfo = static_cast<kodgen::NamespaceInfo const&>(entityInfo);

	//Hide namespace macro redefinition warning as it is likely to happen often
	generatedFile.writeLines("__RFK_DISABLE_WARNING_PUSH",
							 "__RFK_DISABLE_WARNING_MACRO_REDEFINED\n");

	std::string	mainMacroName						= internalPrefix + getCurrentEntityId() + "_GENERATED";

	std::string fillNamespaceDeclarationMacroName	= generateGetNamespaceFragmentDeclarationMacro(generatedFile, namespaceInfo);
	std::string fillNamespaceDefinitionMacroName	= generateGetNamespaceFragmentDefinitionMacro(generatedFile, namespaceInfo);
	std::string registerMacroName					= generateRegistrationMacro(generatedFile, namespaceInfo);

	//Use parsing macro to avoid parsing generated data
	generatedFile.writeLine("#ifdef " + kodgen::FileParser::parsingMacro);

	generatedFile.writeMacro(std::string(mainMacroName));

	generatedFile.writeLine("#else");

	generatedFile.writeMacro(std::move(mainMacroName),
							 "namespace rfk::generated {",
							 std::move(fillNamespaceDeclarationMacroName),
							 std::move(registerMacroName),
							 std::move(fillNamespaceDefinitionMacroName),
							 "}");

	generatedFile.writeLine("#endif\n");

	generatedFile.writeLine("__RFK_DISABLE_WARNING_POP\n");
}

std::string GeneratedNamespaceCodeTemplate::generateGetNamespaceFragmentDeclarationMacro(kodgen::GeneratedFile& generatedFile, kodgen::NamespaceInfo const& /* namespaceInfo */) const noexcept
{
	std::string macroName = internalPrefix + getCurrentEntityId() + "_DeclareGetNamespaceFragment";

	generatedFile.writeLines("#define " + macroName + " inline rfk::NamespaceFragment const& " + getGetNamespaceFragmentFunctionName(generatedFile) + "() noexcept;",
							 "");

	return macroName;
}

std::string GeneratedNamespaceCodeTemplate::generateGetNamespaceFragmentDefinitionMacro(kodgen::GeneratedFile& generatedFile, kodgen::NamespaceInfo const& namespaceInfo) const noexcept
{
	std::string macroName = internalPrefix + getCurrentEntityId() + "_DefineGetNamespaceFragment";

	generatedFile.writeLines("#define " + macroName + "\t\\",
							 "	inline rfk::NamespaceFragment const& " + getGetNamespaceFragmentFunctionName(generatedFile) + "() noexcept\t\\",
							 "	{\t\\",
							 "		static rfk::NamespaceFragment	fragment(\"" + namespaceInfo.name + "\", " + getCurrentEntityId() + ");\t\\",
							 "		static bool						initialized = false;\t\\",
							 "		if (!initialized)\t\\",
							 "		{\t\\",
							 "			initialized = true;\t\\"
							);

	//Fill namespace properties
	std::string properties = fillEntityProperties(namespaceInfo, "fragment.");
	if (!properties.empty())
	{
		generatedFile.writeLine("			" + properties + "\t\\");
	}

	kodgen::uint64 totalSize = namespaceInfo.namespaces.size() +
								namespaceInfo.structs.size() +
								namespaceInfo.classes.size() +
								namespaceInfo.enums.size() +
								namespaceInfo.variables.size() +
								namespaceInfo.functions.size();

	//Fill nested namespaces
	if (totalSize != 0u)
	{
		//Reserve space first
		generatedFile.writeLine("			fragment.nestedEntities.reserve(" + std::to_string(totalSize) + "u);\t\\");

		//Fill nested namespaces
		for (kodgen::NamespaceInfo const& nestedNamespace : namespaceInfo.namespaces)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(" + getNamespaceFragmentRegistererName(generatedFile, nestedNamespace) + ".getNamespaceInstance());\t\\");
		}

		//Fill nested structs
		for (kodgen::StructClassInfo const& nestedStruct : namespaceInfo.structs)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(&" + nestedStruct.type.getCanonicalName() + "::staticGetArchetype());\t\\");
		}

		//Fill nested classes
		for (kodgen::StructClassInfo const& nestedClass : namespaceInfo.classes)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(&" + nestedClass.type.getCanonicalName() + "::staticGetArchetype());\t\\");
		}

		//Fill nested enums
		for (kodgen::EnumInfo const& nestedEnum : namespaceInfo.enums)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(rfk::getEnum<" + nestedEnum.type.getCanonicalName() + ">());\t\\");
		}

		//Fill nested variables
		for (kodgen::VariableInfo const& variable : namespaceInfo.variables)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(&" + GeneratedVariableCodeTemplate::getGetVariableFunctionName(variable) + "());\t\\");
		}

		//Fill nested functions
		for (kodgen::FunctionInfo const& function : namespaceInfo.functions)
		{
			generatedFile.writeLine("			fragment.nestedEntities.emplace_back(&" + GeneratedFunctionCodeTemplate::getGetFunctionFunctionName(function) + "());\t\\");
		}
	}

	generatedFile.writeLines("		}\t\\",
							 "	return fragment;\t\\",
							 "	}",
							 "");

	return macroName;
}

std::string GeneratedNamespaceCodeTemplate::generateRegistrationMacro(kodgen::GeneratedFile& generatedFile, kodgen::NamespaceInfo const& namespaceInfo) const noexcept
{
	std::string macroName = internalPrefix + getCurrentEntityId() + "_RegisterNamespace";

	generatedFile.writeLines("#define " + macroName + "\t\\",
								"	inline rfk::NamespaceFragmentRegisterer " + getNamespaceFragmentRegistererName(generatedFile, namespaceInfo) +
								" = rfk::NamespaceFragmentRegisterer(\"" +
								namespaceInfo.name + "\", " +
								getCurrentEntityId() + ", " +
								"&" + getGetNamespaceFragmentFunctionName(generatedFile) + "(), " +
								((namespaceInfo.outerEntity == nullptr) ? "true" : "false") +
								");");

	generatedFile.writeLine("");

	return macroName;
}

std::string GeneratedNamespaceCodeTemplate::getGetNamespaceFragmentFunctionName(kodgen::GeneratedFile const& generatedFile) const noexcept
{
	return "getNamespaceFragment" + getCurrentEntityId() + "_" + std::to_string(stringHasher(generatedFile.getSourceFilePath().string()));
}

std::string GeneratedNamespaceCodeTemplate::getNamespaceFragmentRegistererName(kodgen::GeneratedFile const& generatedFile, kodgen::NamespaceInfo const&	namespaceInfo) const noexcept
{
	return "namespaceFragmentRegisterer" + std::to_string(stringHasher(namespaceInfo.id)) + "u_" + std::to_string(stringHasher(generatedFile.getSourceFilePath().string()));
}