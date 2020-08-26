
/// written by Hossein Nazari, 15 August 2020

#include "pseudo_unorderd_map.h"
#include "validation_test.h"
#include "data_test.h"

#include <gtest/gtest.h>
#include <hiredis/hiredis.h>
#include <boost/lexical_cast.hpp>
#include <string>


#define REDIS_IP "127.0.0.1"
#define REDIS_PORT 6379


DataTest data_generator;

template <typename TYPE>
std::string to_string(TYPE data)
{
    return boost::lexical_cast<std::string>(data); 
}



typedef ::testing::Types <
                          TypeTuple<int, int>,
                          TypeTuple<int, char>,
                          TypeTuple<int, double>,
                          TypeTuple<int, bool>,
                          TypeTuple<int, std::string>,
                          TypeTuple<char, int>, 
                          TypeTuple<char, char>,
                          TypeTuple<char, double>,
                          TypeTuple<char, bool>,
                          TypeTuple<char, std::string>,
                          TypeTuple<double, int>,   
                          TypeTuple<double, char>,
                          TypeTuple<double, double>,
                          TypeTuple<double, bool>,
                          TypeTuple<double, std::string>,
                          TypeTuple<bool, int>,
                          TypeTuple<bool, char>,
                          TypeTuple<bool, double>,
                          TypeTuple<bool, bool>,
                          TypeTuple<bool, std::string>,
                          TypeTuple<std::string, int>, 
                          TypeTuple<std::string, char>,
                          TypeTuple<std::string, double>,   
                          TypeTuple<std::string, bool>,
                          TypeTuple<std::string, std::string>
                           > Implementations;



TYPED_TEST_CASE(PseudoUnorderedMapTest, Implementations);


TYPED_TEST(PseudoUnorderedMapTest, SizeMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    std::string key, value;
  
    redisCommand(this->redis_context, "FLUSHDB");
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_EQ(0, this->test_map->size()); 
    
    key = to_string(data_generator.get_data<KEY_TYPE>());
    value = to_string(data_generator.get_data<VAL_TYPE>());
    redisCommand(this->redis_context, "SET %s %s", key.c_str(), value.c_str()); 
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_EQ(1, this->test_map->size()); 
    
    int few_times = 10;
    redisCommand(this->redis_context, "FLUSHDB");
    for (int count = 0; count < few_times; count++)
    {
        key = to_string(data_generator.get_data<KEY_TYPE>());
        value = to_string(data_generator.get_data<VAL_TYPE>());
        redisCommand(this->redis_context, "SET %s %s", key.c_str(), value.c_str());
    }
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_EQ(this->redis_reply->integer, this->test_map->size()); 

    redisCommand(this->redis_context, "DEL %s", key.c_str());
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_EQ(this->redis_reply->integer, this->test_map->size());  


    redisCommand(this->redis_context, "FLUSHDB");
    redisCommand(this->redis_context, "DEL %s", key.c_str());
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_EQ(this->redis_reply->integer, this->test_map->size());  

}


TYPED_TEST(PseudoUnorderedMapTest, EmptyMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    std::string key, value;
  
    redisCommand(this->redis_context, "FLUSHDB");
    EXPECT_TRUE(this->test_map->empty());

    redisCommand(this->redis_context, "FLUSHDB");
    key = to_string(data_generator.get_data<KEY_TYPE>());
    value = to_string(data_generator.get_data<VAL_TYPE>());
    redisCommand(this->redis_context, "SET %s %s", key.c_str(), value.c_str());
    EXPECT_FALSE(this->test_map->empty());

    redisCommand(this->redis_context, "DEL %s", key.c_str()); 
    EXPECT_TRUE(this->test_map->empty());
}




TYPED_TEST(PseudoUnorderedMapTest, ClearMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    std::string key, value;
 
    redisCommand(this->redis_context, "FLUSHDB");
    key = to_string(data_generator.get_data<KEY_TYPE>());
    value = to_string(data_generator.get_data<VAL_TYPE>());
    redisCommand(this->redis_context, "SET %s %s", key.c_str(), value.c_str());
    this->test_map->clear();
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_TRUE(this->redis_reply->integer == 0);

    redisCommand(this->redis_context, "FLUSHDB");
    int few_times = 10;
    for (int count = 0; count < few_times; count++)
    {
        key = to_string(data_generator.get_data<KEY_TYPE>());
        value = to_string(data_generator.get_data<VAL_TYPE>());
        redisCommand(this->redis_context, "SET %s %s", key.c_str(), value.c_str());

    }
    this->test_map->clear();
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_TRUE(this->redis_reply->integer == 0);

    redisCommand(this->redis_context, "FLUSHDB");
    this->test_map->clear();
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "DBSIZE");
    EXPECT_TRUE(this->redis_reply->integer == 0);
 
}


TYPED_TEST(PseudoUnorderedMapTest, InsertMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    VAL_TYPE value;
    
    int few_times = 10;
    redisCommand(this->redis_context, "FLUSHDB");
    for (int count = 0; count < few_times; count++)
    {
        key = data_generator.get_data<KEY_TYPE>();
        value = data_generator.get_data<VAL_TYPE>();
        this->test_map->insert(key, value);
        this->redis_reply = (redisReply*)redisCommand(this->redis_context, "GET %s", to_string(key).c_str());
        VAL_TYPE actual_value = boost::lexical_cast<VAL_TYPE>(this->redis_reply->str);
        EXPECT_EQ(value, actual_value);
    }
}




TYPED_TEST(PseudoUnorderedMapTest, InsertDuplicateKey)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    VAL_TYPE value1, value2;
    
    redisCommand(this->redis_context, "FLUSHDB");
    key = data_generator.get_data<KEY_TYPE>();
    value1 = data_generator.get_data<VAL_TYPE>();
    value2 = data_generator.get_data<VAL_TYPE>();
    
    this->test_map->insert(key, value1);
    this->test_map->insert(key, value2);
    
    this->redis_reply = (redisReply*)redisCommand(this->redis_context, "GET %s", to_string(key).c_str());
    VAL_TYPE actual_value = boost::lexical_cast<VAL_TYPE>(this->redis_reply->str);
    EXPECT_EQ(value2, actual_value);
}



TYPED_TEST(PseudoUnorderedMapTest, AtMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    VAL_TYPE value;

    redisCommand(this->redis_context, "FLUSHDB");
    int few_times = 10;
    for (int count = 0; count < few_times; count++)
    {
        key = data_generator.get_data<KEY_TYPE>();
        value = data_generator.get_data<VAL_TYPE>();
        redisCommand(this->redis_context, "SET %s %s", to_string(key).c_str(), to_string(value).c_str());
        EXPECT_EQ(value, this->test_map->at(key));
    }
}



TYPED_TEST(PseudoUnorderedMapTest, GetKeyWhenNotExistsInDataBase)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    
    redisCommand(this->redis_context, "FLUSHDB");
    int few_times = 10;
 
    for (int count = 0; count < few_times; count++)
    {
        key = data_generator.get_data<KEY_TYPE>();
        EXPECT_THROW(this->test_map->at(key), KeyNotFoundException);
    }
}



TYPED_TEST(PseudoUnorderedMapTest, RemoveMethodWorks)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    VAL_TYPE value;
    redisCommand(this->redis_context, "FLUSHDB");
    int few_times = 10;
    for (int count = 0; count < few_times; count++)
    {
         key = data_generator.get_data<KEY_TYPE>();
         value = data_generator.get_data<VAL_TYPE>();
         redisCommand(this->redis_context, "SET %s %s", to_string(key).c_str(), to_string(value).c_str());
         bool result = this->test_map->remove(key);
         redisReply temp_reply = *(redisReply*)redisCommand(this->redis_context, "EXISTS %s", to_string(key).c_str());
         EXPECT_TRUE(temp_reply.integer == 0);
         EXPECT_TRUE(result);
         
    }
}



TYPED_TEST(PseudoUnorderedMapTest, RemoveKeyNotInDataBase)
{
    typedef typename TypeParam::KEY_TYPE KEY_TYPE;
    typedef typename TypeParam::VAL_TYPE VAL_TYPE;

    KEY_TYPE key;
    redisCommand(this->redis_context, "FLUSHDB");
    key = data_generator.get_data<KEY_TYPE>();
    bool result = this->test_map->remove(key);
    EXPECT_FALSE(result);
}



int main(int argc, char **argv)
{   
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

