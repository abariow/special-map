
#include "performance_test.h"
#include "pseudo_unorderd_map.h"
#include "data_test.h"
#include <pthread.h>
#include <vector>
#include <chrono>

#define REDIS_IP "127.0.0.1"
#define REDIS_PORT 6379
#define SOCKET_PATH "/var/run/redis/redis.sock"


const size_t key_size = 30;
const size_t value_size = 1000;
std::vector<DATA> data_test;
MAP** maps;

std::string connection_type[3] = {"TCP", "UnixSocket", "SharedMemory"};
std::string request_type[2] = {"SET", "GET"};


double performanceTest(size_t request_num, size_t thread_num, RequestType RT, ConnectionType CT)
{

	MAP temp_map(REDIS_IP, REDIS_PORT);
	temp_map.clear();

	maps = buildMaps(thread_num, CT);
	generateDataTest(data_test, thread_num, request_num);
	
	void * (*thread_function)(void *);

	switch (RT)
	{
		case SET:
			thread_function = threadSetData;		
			break;
		case GET:
			thread_function = threadGetData;
			break;
	}

	double *results;
	results = handleThreads(thread_num, thread_function);
	double RPS = calculateRPS(results, thread_num, request_num);
	double threads_mean_time = calculateThreadsMeanTime(results, thread_num);
	
	TestResults(RPS, threads_mean_time, request_num, thread_num, RT, CT);

	delete results;
	deleteMaps(thread_num);

	return RPS;
}


MAP** buildMaps(size_t maps_num, ConnectionType CT)
{
	MAP** maps = new MAP* [maps_num];
	if (CT == TCP) 
		for (int count = 0; count < maps_num; count++)
	       maps[count] = new MAP(REDIS_IP, REDIS_PORT);	
		   
 	else
		for (int count = 0; count < maps_num; count++)
		   maps[count] = new MAP(SOCKET_PATH, CT);
		   
    return maps;
}


void deleteMaps(size_t maps_num)
{
	for (int count = 0; count < maps_num; count++)
		delete maps[count];
	delete maps;
}


DATA generateKeyValue(size_t size, size_t key_size, size_t value_size)
{
    DATA data;
    DataTest data_generator;
    std::string key = "";
    std::string sub_key = data_generator.get_string(16); 
    long long r = 1000000000000000;
    std::string value = "";
    for (int i = 0; i < 1000; i++)
        value += "0";
	
    for (int count = 0; count < size; count++)
    {
        key = sub_key + std::to_string(r); 
        r += 1;
        struct Tuple<std::string, std::string> pair = {key, value};
        data.push_back(pair);              
    }
            
    return data;      
}


void generateDataTest(std::vector<DATA> &data_test, size_t data_num, size_t data_size)
{
	for (int count = 0; count < data_num; count++)
		data_test.push_back(generateKeyValue(data_size, key_size, value_size));
}




void setData(MAP &map, const DATA &data)
{
    for (int idx = 0; idx < data.size(); idx++)
    	map.insert(data[idx].key, data[idx].value); 
	
}
        

void getData(MAP &map, const DATA &data)
{
    for (int idx = 0; idx < data.size(); idx++)
        map.at(data[idx].key);
}

        
void *threadGetData(void *arg)
{ 
    int thread_id = *((int *)arg);
    setData(*maps[thread_id], data_test[thread_id]);
    auto start = std::chrono::high_resolution_clock::now();   
    getData(*maps[thread_id], data_test[thread_id]);    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double>elapsed_time = end - start;
    double *time = new double(elapsed_time.count());
    return (void *)time;
}
       
        
 
void *threadSetData(void *arg)
{
	int thread_id = *((int *)arg);
    auto start = std::chrono::high_resolution_clock::now();  
    setData(*maps[thread_id], data_test[thread_id]);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    double *time = new double(elapsed_time.count());
    return (void *)time;    
}
        

double *handleThreads(size_t thread_num, void * (*thread_function)(void *))
{
	
	double *results = new double[thread_num];
	double thread_result;
    void *temp; 

    pthread_t threads[thread_num];
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int *arg;
	for (int count = 0; count < thread_num; count++)
	{	
		arg = new int;
		*arg = count;
    	pthread_create(&threads[count], &attr, thread_function, arg);
	}
	
	pthread_attr_destroy(&attr);

    for (int count = 0; count < thread_num; count++) 
    {
        temp = NULL;
        pthread_join(threads[count], &temp);
        thread_result = *((double*)temp);
		results[count] = thread_result;
    }

	delete temp;
   	delete arg;
    return results;
}
        

/// RPS = Request Per Second
double calculateRPS(double *threads_result, size_t thread_num, size_t request_num)
{
	double RPS = 0.0;
	for (int count = 0; count < thread_num; count++)
		RPS += (request_num / threads_result[count]);
	return RPS;
}


double calculateThreadsMeanTime(double *threads_result, size_t thread_num)
{
	double total_time = 0.0;
	for (int count = 0; count < thread_num; count++)
		total_time += threads_result[count];
	return (total_time / thread_num);
}


void TestResults(double RPS, double mean, size_t request_num, size_t thread_num, RequestType RT, ConnectionType CT)
{
	
	std::cout << ".........Test Parameters........"<< std::endl;
	std::cout << "Thread Numbers: " << thread_num << std::endl;
	std::cout << "Request Numbers(for each thread): " << request_num << std::endl;
	std::cout << "Request Type: " << request_type[RT] << std::endl;
	std::cout << "Connection Type: " << connection_type[CT] << std::endl << std::endl;

	std::cout << ".........Test Results........"<< std::endl;
	std::cout << "RPS(Request Per Second): " << (int)RPS / 1000 << "K" << std::endl;
	std::cout << "Thread Mean Time (s): " << mean << std::endl << std::endl;
}



