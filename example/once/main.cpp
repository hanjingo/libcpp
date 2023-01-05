#include <iostream>

#include <libcpp/sync/once.hpp>

void hello()
{
    std::cout << "hello once before" << std::endl;
	ONCE(
		std::cout << "hello" << std::endl;
	);
    std::cout << "hello once after" << std::endl;
}

void world()
{
    std::cout << "world once before" << std::endl;
	ONCE(
		std::cout << "world" << std::endl;
	);
    std::cout << "world once before" << std::endl;
}

/*
hello once before
hello
hello once after

hello once before
hello once after

world once before
world
world once before

world once before
world once before
*/
int main(int argc, char* argv[])
{
	hello();
    std::cout << std::endl;

	hello();
    std::cout << std::endl;

    world();
    std::cout << std::endl;

    world();
    std::cout << std::endl;

    std::cin.get();
    return 0;
}