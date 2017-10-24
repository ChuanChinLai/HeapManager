#include <algorithm>
#include <iostream>

#include <HeapManager.h>
#include <MemoryManager.h>

int main(int argc, const char * argv[])
{
	Engine::Memory::_Init();
	Engine::Memory::HeapManager* pHeap = Engine::Memory::HeapManager::_Get();
    

	Engine::Memory::HeapManager_UnitTest();

    return 0;
}
