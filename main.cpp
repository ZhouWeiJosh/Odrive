#include "endpoint.hpp"
#include <iostream>
#include <cstring>

int main() 
{
	odrive_endpoint endpoint;

	uint64_t serialNumber = 0x2052325E4D31;

	int result = endpoint.init(serialNumber);

	if(result == 0)
	{
		std::cout << "ODrive initialized successfully." << std::endl;
	}
	else
	{
		std::cerr << "Failed to initialize ODrive. Error code: " << result << std::endl;
	}

	return 0;
}	
