#include "Trigger.h"
#include "Manager.h"
#include "Agent.h"

int main()
{
	kiwi::Manager<int, kiwi::TriggerBase<int, int>> man;

	kiwi::TriggerAgent<int, int> ta;

	man(ta(3));

	return 0;
}
