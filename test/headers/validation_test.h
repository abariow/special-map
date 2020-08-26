#ifndef PSEUDO_UNORDERD_MAP_TEST_H
#define PSEUDO_UNORDERD_MAP_TEST_H

#include "pseudo_unorderd_map.h"
#include "data_test.h"
#include <gtest/gtest.h>
#include <pthread.h>
#include <hiredis/hiredis.h>
#include <vector>
#include <chrono>

#define REDIS_IP "127.0.0.1"
#define REDIS_PORT 6379


template <typename TYPE1, typename TYPE2>
struct TypeTuple
{
    typedef TYPE1 KEY_TYPE;
    typedef TYPE2 VAL_TYPE;
};



template <class T>
class PseudoUnorderedMapTest : public ::testing::Test
{
	typedef typename T::KEY_TYPE KEY_TYPE;
	typedef typename T::VAL_TYPE VAL_TYPE;

    protected:
         
        virtual void SetUp()
        {
            redis_context = redisConnect(REDIS_IP, REDIS_PORT);
            test_map = new PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>(REDIS_IP, REDIS_PORT);   
        }

        virtual void TestBody()
        {
        }

        virtual void TearDown() 
        {
            freeReplyObject(redis_reply);
            redisFree(redis_context);
            delete test_map;
        }

        PseudoUnorderedMap<KEY_TYPE, VAL_TYPE> *test_map;
        redisContext *redis_context; //!< to hold connection state 
        redisReply *redis_reply = nullptr; //!< to hold redis replies
};


#endif
