#include "System.h"

using namespace ECSEngine;

std::string_view SystemBase::getName() const
{
    return "NONE";
}

float SystemBase::getFrameTime() const
{
    return 0.0f;
}

bool SystemBase::shouldTick(EntityRegistry& registry) const
{
    return false;
}

void SystemBase::initialize(EntityRegistry& registry, SystemContainer& systemContainer)
{
    if (m_bIsInitialized)
    {
        return;
    }

    onInitialize(registry, systemContainer);
    m_bIsInitialized = true;
}

void SystemBase::deinitialize(EntityRegistry& registry)
{
    if (!m_bIsInitialized)
    {
        return;
    }

    onDeinitialize(registry);
    m_bIsInitialized = false;
}

void SystemBase::tick(float deltaTime, EntityRegistry& registry)
{
}

void SystemBase::onInitialize(EntityRegistry& registry, SystemContainer& systemContainer)
{
}

void SystemBase::onDeinitialize(EntityRegistry& registry)
{
}
