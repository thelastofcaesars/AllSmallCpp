#include <olcConsoleGameEngine.h>

class tLoCEngine3D : public olcConsoleGameEngine
{
public:
	tLoCEngine3D()
	{

	}

public:
	bool OnUserCreate() override
	{

		return true;
	}
	bool OnUserUpdate(float fEnlapsedTime) override
	{

		return true;
	}
};

int main()
{
	tLoCEngine3D demo;
	if (demo.ConstructConsl)
		
		
	return 0;
}
