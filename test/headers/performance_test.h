#ifndef PERFORMANCE_TEST_H  
#define PERFORMANCE_TEST_H

#include "pseudo_unorderd_map.h"
#include <vector>


template <typename KEY_TYPE, typename VAL_TYPE>
struct Tuple
{
    KEY_TYPE key;
    VAL_TYPE value;
};


enum RequestType
{
	SET, GET
};


typedef std::vector<Tuple<std::string, std::string>> DATA;
typedef PseudoUnorderedMap<std::string, std::string> MAP;


extern const size_t key_size;
extern const size_t value_size;
extern std::vector<DATA> data_test;
extern MAP** maps;


DATA generateKeyValue(size_t size, size_t key_size, size_t value_size);
void generateDataTest(std::vector<DATA> &data_test, size_t data_num, size_t data_size);
double performanceTest(size_t request_num, size_t thread_num, RequestType RT, ConnectionType CT);
MAP** buildMaps(size_t maps_num, ConnectionType CT); 
void deleteMaps(size_t maps_num); 
void setData(const MAP &map, const DATA &data);
void getData(const MAP &map, const DATA &data);
void *threadSetData(void *arg);
void *threadGetData(void *arg);
double *handleThreads(size_t thread_num, void * (*thread_function)(void *));
double calculateRPS(double *threads_result, size_t thread_num, size_t request_num);
double calculateThreadsMeanTime(double *threads_result, size_t thread_num);
void TestResults(double RPS, double mean, size_t request_num, size_t thread_num, RequestType RT, ConnectionType CT);

#endif
