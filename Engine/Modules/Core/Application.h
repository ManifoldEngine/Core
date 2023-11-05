#pragma once

#include <Core/Core.h>

namespace ECSEngine
{
	class SystemContainer;

	class Core_API Application
	{
	public:
		Application();
		~Application();

		static Application& get();

		void run();
		void stop();

		// Inherited via ITickable
		void tick(float deltaTime);
	
		SystemContainer& getSystemContainer();
		bool isRunning() const { return m_isRunning; }

	private:
		static Application* sm_application;

		bool m_isRunning = false;
		
		SystemContainer* m_systemContainer = nullptr;
	};
}