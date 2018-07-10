
#include <iostream>


int main(){

	std::cout << "Starting game..." << std::endl;

	Game::getInstance().Run();
	
	std::cout << "Exiting game..." << std::endl;
	
	return 0;  

}

