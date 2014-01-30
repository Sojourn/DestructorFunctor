#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>

#include "destructor.h"

template<class T, class Arg>
T *custom_new(const Arg &arg)
{
	return new(malloc(sizeof(std::string))) std::string(arg);
}

void custom_delete(void *mem, Destructor destructor)
{
	destructor(mem);
	free(mem);
}

int main(int argc, char **argv)
{
	std::string *str1 = custom_new<std::string>("Hello");
	std::string *str2 = custom_new<std::string>(", World!");
		
	std::cout << *str1 << *str2 << std::endl;

	Destructor destructor;
	destructor.attune<std::string>();

	custom_delete(str1, destructor);
	custom_delete(str2, destructor);

#ifdef _WIN32
	std::system("pause");
#endif
	return 0;
}