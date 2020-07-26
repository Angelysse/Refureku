#include "TypeInfo/Namespaces/NamespaceFragmentRegisterer.h"

#include <cassert>

#include "TypeInfo/Namespaces/Namespace.h"
#include "TypeInfo/Namespaces/NamespaceFragment.h"
#include "TypeInfo/Database.h"

using namespace rfk;

NamespaceFragmentRegisterer::NamespaceFragmentRegisterer(char const* name, uint64 id, NamespaceFragment const* namespaceFragment, bool isFileLevelNamespace) noexcept
{
	//Try to get an existing namespace
	Entity const* foundNamespace = Database::getEntity(id);

	if (foundNamespace == nullptr)
	{
		//This namespace hasn't been registered yet, so create it
		_namespaceInstance = new Namespace(name, id);	//TODO: The database should be the one generating the pointer, (using shared_ptr)
		_isNamespaceOwner	= true;

		if (isFileLevelNamespace)
		{
			//Don't register a namespace sub entities as they will be registered manually during fragment merging
			Database::registerFileLevelEntity(*_namespaceInstance, false);
		}
	}
	else
	{
		//Sorry we need to get rid of that const to fill you...
		_namespaceInstance = const_cast<Namespace*>(reinterpret_cast<Namespace const*>(foundNamespace));
	}

	mergeFragmentToNamespace(namespaceFragment);
}

NamespaceFragmentRegisterer::~NamespaceFragmentRegisterer() noexcept
{
	if (_isNamespaceOwner)
	{
		delete _namespaceInstance;
	}
}

void NamespaceFragmentRegisterer::mergeFragmentToNamespace(NamespaceFragment const* fragment) noexcept
{
	//Make sure we can actually merge this fragment to the namespace
	assert(_namespaceInstance->id == fragment->id);

	_mergedNamespaceFragment = fragment;

	//Merge properties
	mergeFragmentPropertiesToNamespaceProperties(fragment);

	for (Entity const* entity : fragment->nestedEntities)
	{
		switch (entity->kind)
		{
			case Entity::EKind::Namespace:
				_namespaceInstance->nestedNamespaces.emplace(reinterpret_cast<Namespace const*>(entity));

				//Register the nested namespace but don't register sub entities
				Database::registerEntityById(*entity, false);

				break;

			case Entity::EKind::Archetype:
				_namespaceInstance->nestedArchetypes.emplace(reinterpret_cast<Archetype const*>(entity));
				
				//Register the archetype and its sub entities to the database.
				Database::registerEntityById(*entity, true);

				break;

			case Entity::EKind::NamespaceFragment:
				[[fallthrough]];
			case Entity::EKind::EnumValue:
				[[fallthrough]];
			case Entity::EKind::Field:
				[[fallthrough]];
			case Entity::EKind::Method:
				[[fallthrough]];
			case Entity::EKind::Undefined:
				[[fallthrough]];
			default:
				assert(false);	//None of these kind of entities should ever be a namespace nested entity
				break;
		}
	}
}

void NamespaceFragmentRegisterer::mergeFragmentPropertiesToNamespaceProperties(NamespaceFragment const* fragment) noexcept
{
	//Append simple properties
	for (std::string const& simpleProperty : fragment->properties.simpleProperties)
	{
		_namespaceInstance->properties.simpleProperties.insert(simpleProperty);
	}

	//Append complex properties
	for (auto& [key, value] : fragment->properties.complexProperties)
	{
		if (!_namespaceInstance->properties.hasProperty(key, value))
		{
			_namespaceInstance->properties.complexProperties.insert(std::make_pair(key, value));
		}
	}
}

void NamespaceFragmentRegisterer::removeFragmentFromNamespace(NamespaceFragment const* fragment) noexcept
{
	//TODO
}

Namespace const* NamespaceFragmentRegisterer::getNamespaceInstance() const noexcept
{
	return _namespaceInstance;
}