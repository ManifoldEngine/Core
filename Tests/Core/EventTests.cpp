#include "simpleTests.h"
#include "Events/Event.h"

using namespace ECSEngine;

ST_TEST(SubscribeAndUnsubscribe, "Should be able to call back the delegate it receives")
{
	Event<int> someEvent;
	bool bWasCalled = false;
	Event<int>::Handle handle = someEvent.subscribe([&bWasCalled](int i) {
		ST_ASSERT(i == 4, "argument should be equal to what was passed.");
		bWasCalled = true;
	});

	someEvent.broadcast(4);
	ST_ASSERT(bWasCalled, "delegate should have been called");

	bWasCalled = false;
	someEvent.unsubscribe(handle);

	someEvent.broadcast(4);

	ST_ASSERT(!bWasCalled, "delegate should have been called.");
}

ST_TEST(SubscribeAndUnsubscribeFromFuncRef, "Should be able to call back the delegate it receives, the delegate being a function ref")
{
	class SomeClass
	{
	public:
		bool bWasCalled = false;

		void onSomeEvent(int i)
		{
			ST_ASSERT(i == 4, "argument should be equal to what was passed.");
			bWasCalled = true;
		};
	};

	Event<int> someEvent;

	SomeClass someObject;

	Event<int>::Handle handle = someEvent.subscribe([&someObject](int i) {
		ST_ASSERT(i == 4, "argument should be equal to what was passed.");
		someObject.onSomeEvent(i);
	});

	someEvent.broadcast(4);
	ST_ASSERT(someObject.bWasCalled, "delegate should have been called");

	someObject.bWasCalled = false;
	someEvent.unsubscribe(handle);

	someEvent.broadcast(4);

	ST_ASSERT(!someObject.bWasCalled, "delegate should have been called.");
}

ST_TEST(SubscribeAndUnsubscribeMultipleTimes, "Should be able to call back the all delegates it receives")
{
	class SomeClass
	{
	public:
		bool bWasCalled = false;

		void onSomeEvent(int i)
		{
			ST_ASSERT(i == 4, "argument should be equal to what was passed.");
			bWasCalled = true;
		};
	};

	std::vector<SomeClass> listeners;
	for (int i = 0; i < 10; i++)
	{
		listeners.push_back(SomeClass());
	}

	Event<int> someEvent;
	std::vector<Event<int>::Handle> handles;
	for (auto& someObject : listeners)
	{
		auto callback = [&someObject](int i) {
			ST_ASSERT(i == 4, "argument should be equal to what was passed.");
			someObject.onSomeEvent(i);
		};
		handles.push_back(someEvent.subscribe(callback));
	}

	someEvent.broadcast(4);

	bool bWasCalled = true;
	for (const auto& someObject : listeners)
	{
		bWasCalled &= someObject.bWasCalled;
	}

	ST_ASSERT(bWasCalled, "all delegates should have been called");

	for (const auto& handle : handles)
	{
		someEvent.unsubscribe(handle);
	}
}