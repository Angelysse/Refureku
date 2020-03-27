#pragma once

#include <vector>

#include <CodeGen/GeneratedCodeTemplate.h>
#include <InfoStructures/StructClassInfo.h>
#include <InfoStructures/EnumInfo.h>

namespace refureku
{
	class GeneratedCodeTemplate : public kodgen::GeneratedCodeTemplate
	{
		private:
			static constexpr char const*	_internalMacroPrefix = "__RFK";
			static constexpr char const*	_externalMacroPrefix = "RFK";
			static std::hash<std::string>	_stringHasher;

			void			undefMacros(kodgen::GeneratedFile& generatedFile, std::string const& generatedMacroName)					const	noexcept;

			void			generateClassCode(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& classInfo)			const	noexcept;
			void			generateStructCode(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& structInfo)			const	noexcept;
			void			generateEnumCode(kodgen::GeneratedFile& generatedFile, kodgen::EnumInfo const& enumInfo)					const	noexcept;

			void			sortMethods(std::vector<kodgen::MethodInfo> const&	allMethods,
										std::vector<kodgen::MethodInfo const*>& out_methods,
										std::vector<kodgen::MethodInfo const*>& out_staticMethods)										const	noexcept;

			/** Return generated macro name */
			std::string		generateGetArchetypeMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info)		const	noexcept;
			std::string		generateDefaultInstantiateMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info)	const	noexcept;
			std::string		generateParentsMetadataMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info)		const	noexcept;
			std::string		generateMethodsMetadataMacro(kodgen::GeneratedFile& generatedFile, kodgen::StructClassInfo const& info)		const	noexcept;

		protected:
			virtual void	generateCode(kodgen::GeneratedFile& generatedFile, kodgen::EntityInfo const& entityInfo)					const	noexcept override;
	};
}
