#ifdef ECSE_TEST

#include "simpleTests.h"
#include "ECS/EntityRegistry.h"
#include "ECS/RegistryView.h"

using namespace ECSEngine;

ST_TEST(CreateEditDestroyEntities, "Should create a registry, create an entity, add a component, edit the component, then destroy the entity")
{
	struct DataComponent 
	{
		int someData = 5;
	};

	// create a registry
	EntityRegistry registry;

	// create an entity
	EntityId entityId = registry.create();
	
	ST_ASSERT(entityId == 0, "Entity should have the first id.");
	ST_ASSERT(registry.isValid(entityId), "Entity should be valid.");
	
	// add a DataComponent
	auto* pComponent = registry.addComponent<DataComponent>(entityId);
	
	ST_ASSERT(pComponent != nullptr, "Component should not be a nullptr.");
	if (pComponent == nullptr)
	{
		return;
	}

	// edit the data
	pComponent->someData += 10;

	// get the component again to verify the pointer points to the correct location.
	auto* pComponentFromRegistry = registry.getComponent<DataComponent>(entityId);
	
	ST_ASSERT(pComponentFromRegistry != nullptr, "Component should not be a nullptr.");
	if (pComponentFromRegistry == nullptr)
	{
		return;
	}

	ST_ASSERT(pComponent->someData == pComponentFromRegistry->someData, "We should be able to retrieve our component data");
	
	// destroy the entity, should destroy the component too.
	ST_ASSERT(registry.destroy(entityId), "The Entity should have been destroyed");
	
	ST_ASSERT(!registry.isValid(entityId), "Entity should not be valid anymore");

	auto* pComponentFromRegistryAfterDelete = registry.getComponent<DataComponent>(entityId);
	ST_ASSERT(pComponentFromRegistryAfterDelete == nullptr, "Entity's component should be nullptr after delete.");

	EntityRegistry::resetComponentIds();
}

ST_TEST(AddAndRemoveComponents, "Should create a registry and an entity, then add and remove a component")
{
	struct DataComponent
	{
		int someData = 5;
	};

	// create a registry
	EntityRegistry registry;

	// create an entity
	EntityId entityId = registry.create();
	
	ST_ASSERT(entityId == 0, "Should be the first entity addded.");

	// add a DataComponent
	DataComponent* pDataComponent = registry.addComponent<DataComponent>(entityId);
	ST_ASSERT(pDataComponent != nullptr, "Component should not be a nullptr.");
	if (pDataComponent == nullptr)
	{
		return;
	}

	const bool bDidRemoveDataComponent = registry.removeComponent<DataComponent>(entityId);
	ST_ASSERT(bDidRemoveDataComponent, "Should have removed the Data component");

	pDataComponent = registry.getComponent<DataComponent>(entityId);
	ST_ASSERT(pDataComponent == nullptr, "Component should be a nullptr.");
	
	EntityRegistry::resetComponentIds();
}

ST_TEST(RecycleEntities, "Should create a registry and an entity, then properly recycle the entity and its components")
{
	struct DataComponent
	{
		int someData = 5;
	};

	struct OtherDataComponent {
		int someOtherData = 10;
	};

	// create a registry
	EntityRegistry registry;

	// create an entity
	EntityId entityId = registry.create();

	ST_ASSERT(entityId == 0, "Should be the first entity addded.");

	// add a DataComponent
	DataComponent* pDataComponent = registry.addComponent<DataComponent>(entityId);
	ST_ASSERT(pDataComponent != nullptr, "Component should not be a nullptr.");
	if (pDataComponent == nullptr)
	{
		return;
	}

	// destroy the entity, which should be added to the entity pool
	const bool bDidDestroy = registry.destroy(entityId);
	ST_ASSERT(bDidDestroy, "Should have detroyed entity");

	pDataComponent = registry.getComponent<DataComponent>(entityId);
	ST_ASSERT(pDataComponent == nullptr, "Component should be a nullptr.");

	// we expect the entity to be recycled
	entityId = registry.create();
	ST_ASSERT(entityId == 0, "Should be the first entity addded.");

	// Entity should not have a DataComponent
	pDataComponent = registry.getComponent<DataComponent>(entityId);
	ST_ASSERT(pDataComponent == nullptr, "Component should be a nullptr.");

	OtherDataComponent* pOtherDataComponent = registry.addComponent<OtherDataComponent>(entityId);
	ST_ASSERT(pOtherDataComponent != nullptr, "Component should not be a nullptr.");
	if (pOtherDataComponent == nullptr)
	{
		return;
	}

	pOtherDataComponent->someOtherData += 10;

	OtherDataComponent* pOtherOtherDataComponent = registry.getComponent<OtherDataComponent>(entityId);
	ST_ASSERT(pOtherOtherDataComponent != nullptr, "Other Component should not be a nullptr.");
	if (pOtherOtherDataComponent == nullptr)
	{
		return;
	}

	ST_ASSERT(pOtherOtherDataComponent->someOtherData == pOtherDataComponent->someOtherData, "we should be able to mutate a component of a recycled entity.");

	EntityRegistry::resetComponentIds();
}

ST_TEST(CreateRegistryView, "Should create a registry view and iterate through it")
{
	struct DataComponent {
		int someData = 5;
	};

	struct OtherDataComponent {
		int someOtherData = 10;
	};

	// create a registry
	EntityRegistry registry;

	// create e few entities, all of them with DataComponent, some of them with OtherDataComponent
	const int entityCount = 10;
	for (int i = 0; i < entityCount; i++)
	{
		const EntityId entityId = registry.create();
		registry.addComponent<DataComponent>(entityId);
		if (i % 2 == 0)
		{
			registry.addComponent<OtherDataComponent>(entityId);
		}
	}

	ST_ASSERT(registry.size() == entityCount, "should have created 10 entities");

	// iterate over the entities with DataComponent
	int entityCounter = 0;
	for (EntityId entityId : RegistryView<DataComponent>(registry))
	{
		entityCounter++;
		auto* pComponent = registry.getComponent<DataComponent>(entityId);
		ST_ASSERT(pComponent != nullptr, "should have a DataComponent");
	}
	ST_ASSERT(entityCounter == entityCount, "There should be 10 entities with DataComponent in the registry view");

	entityCounter = 0;
	for (EntityId entityId : RegistryView<OtherDataComponent>(registry))
	{
		entityCounter++;
		auto* pOtherComponent = registry.getComponent<OtherDataComponent>(entityId);
		ST_ASSERT(pOtherComponent != nullptr, "should have a OtherDataComponent");
	}
	ST_ASSERT(entityCounter == entityCount / 2, "There should be 5 entities with OtherDataComponent in the registry view");

	entityCounter = 0;
	for (EntityId entityId : RegistryView<DataComponent, OtherDataComponent>(registry))
	{
		entityCounter++;

		auto* pComponent = registry.getComponent<DataComponent>(entityId);
		ST_ASSERT(pComponent != nullptr, "should have a DataComponent");

		auto* pOtherComponent = registry.getComponent<OtherDataComponent>(entityId);
		ST_ASSERT(pOtherComponent != nullptr, "should have a OtherDataComponent");
	}
	ST_ASSERT(entityCounter == entityCount / 2, "There should be 5 entities with DataComponent and OtherDataComponent in the registry view");

	entityCounter = 0;
	for (EntityId entityId : RegistryView<>(registry))
	{
		entityCounter++;
	}
	ST_ASSERT(entityCounter == 10, "Should iterate over all entities");
	
	EntityRegistry::resetComponentIds();
}

ST_TEST(ChaoticEntityCreation, "We should be able to create entities, some without component, and some with components")
{
	struct DataComponent {
		int someData = 5;
	};

	// create a registry
	EntityRegistry registry;

	// create e few entities, all of them with DataComponent, some of them with OtherDataComponent
	const int entityCount = 10;
	for (int i = 0; i < entityCount; i++)
	{
		const EntityId entityId = registry.create();
		if (i % 2 == 0)
		{
			registry.addComponent<DataComponent>(entityId);
		}
	}

	ST_ASSERT(registry.size() == entityCount, "should have created 10 entities");

	// iterate over the entities with DataComponent
	int entityCounter = 0;
	for (EntityId entityId : RegistryView<DataComponent>(registry))
	{
		entityCounter++;
		auto* pComponent = registry.getComponent<DataComponent>(entityId);
		ST_ASSERT(pComponent != nullptr, "should have a DataComponent");
	}
	ST_ASSERT(entityCounter == entityCount / 2, "There should be 5 entities with DataComponent in the registry view");
	
	EntityRegistry::resetComponentIds();
}

ST_TEST(ComponentIdAssignation, "ComponentIds should be consecutive")
{	
	const ComponentId componentId0 = EntityRegistry::getComponentId<struct Component0>();
	const ComponentId componentId1 = EntityRegistry::getComponentId<struct Component1>();
	const ComponentId componentId2 = EntityRegistry::getComponentId<struct Component2>();
		
	ST_ASSERT(componentId1 - componentId0 == 1, "The second componentid should follow the first one");
	ST_ASSERT(componentId2 - componentId1 == 1, "The third componentid should follow the second one");

	EntityRegistry::resetComponentIds();
}
#endif