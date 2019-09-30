#include <hal/board.h>
#include <tuple>


struct A 
{
	int i;
};

int main() 
{
	board_init();

	std::tuple<int, float> tpl = {3, 6.0};

	std::get<0>(tpl);
	std::get<1>(tpl);

	while(1);
}