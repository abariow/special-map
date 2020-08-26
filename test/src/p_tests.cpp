
#include <performance_test.h>
#include <iostream>

int main(int argc, char **argv)
{  
	size_t test_num = 5;
	size_t thread_num = 3;
	size_t request_num = 300000;

	double temp = 0.0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, SET, TCP);

	std::cout << "---------------------------------------------------"<< std::endl;
	std::cout << "SET, TCP  mean RPS: " << (int)(temp / 1000) / test_num << "K" << std::endl;
	std::cout << "---------------------------------------------------"<< std::endl;

	temp = 0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, GET, TCP);

	std::cout << "---------------------------------------------------"<< std::endl;
	std::cout << "GET, TCP  mean RPS: " << (int)(temp / 1000) / test_num << "K" << std::endl;
	std::cout << "---------------------------------------------------"<< std::endl;

	temp = 0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, SET, UNIX_SOCKET);

	std::cout << "---------------------------------------------------"<< std::endl;
	std::cout << "SET, UNIX SOCKET  mean RPS: " << (int)(temp / 1000) / test_num << "K" << std::endl;
	std::cout << "---------------------------------------------------"<< std::endl;

	temp = 0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, GET, UNIX_SOCKET);

	std::cout << "---------------------------------------------------"<<std::endl;
	std::cout << "GET, UNIX SOCKET  mean RPS: " << (int)(temp / 1000) / test_num  << "K" << std::endl;
	std::cout << "---------------------------------------------------"<<std::endl;

	temp = 0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, SET, SHARED_MEMORY);

	std::cout << "---------------------------------------------------"<<std::endl;
	std::cout << "SET, SHARED MEMEORY  mean RPS: " << ((int)temp / 1000) / test_num << "K" << std::endl;
	std::cout << "---------------------------------------------------"<<std::endl;

	temp = 0;
	for (int count = 0; count < test_num; count++)
		temp += performanceTest(request_num, thread_num, GET, SHARED_MEMORY);
	
	std::cout << "---------------------------------------------------"<<std::endl;
	std::cout << "GET, SHARED MEMORY  mean RPS: " << (int)(temp / 1000) / test_num << "K" << std::endl;
	std::cout << "---------------------------------------------------"<<std::endl;


    return 0;;
}


