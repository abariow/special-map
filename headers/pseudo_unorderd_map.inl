
// Written by Hossein Nazari, August 11 2020 


#include "pseudo_unorderd_map.h"

#include <hiredis/hiredis.h>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <cstdlib>



template <typename KEY_TYPE, typename VAL_TYPE>
PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::PseudoUnorderedMap(const std::string& ip_address, unsigned int port)
: redis_reply(nullptr)
{
    /** connects to redis server
     *  it takes ip address and port number of redis server, 
     *  then connects to server, and store connections state
     *  in redis_context
     */
    redis_context = redisConnect(ip_address.c_str(), port);

    /// check if connected to server successfully
    if (redis_context == NULL || redis_context->err)
        throw CouldNotConnectToRedisException();

    // redis_reply = (redisReply*)redisCommand(redis_context, "AUTH %s", password);
 
}


template <typename KEY_TYPE, typename VAL_TYPE>
PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::PseudoUnorderedMap(const std::string& socket_path, ConnectionType ct)
: redis_reply(nullptr)
{
    /** connects to redis server, it takes a socket path,  
     *  then connects to server, and store connections state
     *  in redis_context
     */

	/// checks connection type.
	switch (ct)
	{
		case SHARED_MEMORY:
			redis_context = redisConnectSharedMemory(socket_path.c_str());
			break;

		case UNIX_SOCKET:
    		redis_context = redisConnectUnix(socket_path.c_str());
			break;
		
		case TCP:
			throw CouldNotConnectToRedisException();
	}

    /// checks if connected to server successfully
    if (redis_context == NULL || redis_context->err)
        throw CouldNotConnectToRedisException();
	 
}


 
template <typename KEY_TYPE, typename VAL_TYPE>
VAL_TYPE PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::at(const KEY_TYPE& key)
{ 
    ///make a get command format 
    const char* command = "GET %s";  

    /// converts key to string, no matters what the key type is
    std::string _key = boost::lexical_cast<std::string>(key);  

   
    /// Throws an exception when key is empty
    if (_key == "")
        throw EmptyStringException();
 
    
    /// sends the command to redis, then stores the reply
    redisReply temp_reply = *(redisReply*)redisCommand(redis_context, command, _key.c_str());
    
    /// It throws an exception if the key is not in database.
    if(!temp_reply.str)
        throw KeyNotFoundException(_key);

    /** converts the redis reply into type VAL_TYPE,
     *  VAL_TYPE can be any type
     */
    VAL_TYPE value = boost::lexical_cast<VAL_TYPE>(temp_reply.str);
    return value;
}


template <typename KEY_TYPE, typename VAL_TYPE>
void PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::insert(const KEY_TYPE& key, const VAL_TYPE& value)
{
    /// makes a set command format
    const char* command = "SET %s %s";
    
    /** converts key and value type to string,
     * no matters whate their type are
     */
    std::string _key = boost::lexical_cast<std::string>(key); 
    std::string _value = boost::lexical_cast<std::string>(value); 
  
    /// throws exception when the key or value is empty
    if ((_key == "") || (_value == ""))
        throw EmptyStringException();
 
    /// sends the command to redis server
    redis_reply = (redisReply*)redisCommand(redis_context, command, _key.c_str(), _value.c_str());
    //std::cout << "Inserted successfully" << std::endl;
}


template <typename KEY_TYPE, typename VAL_TYPE>
bool PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::remove(const KEY_TYPE& key)
{
    /// makes a command format of delete
    const char* command = "DEL %s";
    
    /** converts type of key to string,
     * no matters what its type is. 
     */
    std::string _key = boost::lexical_cast<std::string>(key); 

    /// Throws an exception when key is empty
    if (_key == "")
        throw EmptyStringException();

    /// sends the command to redis server
    redis_reply = (redisReply*)redisCommand(redis_context, command, _key.c_str());
    
    return redis_reply->integer != 0;
 }


template <typename KEY_TYPE, typename VAL_TYPE>
size_t PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::size()
{
    /** sends a command to get the size of redis database,
     * No. of key, values
     */
    redis_reply = (redisReply*)redisCommand(redis_context, "DBSIZE");
    return redis_reply->integer;
}


template <typename KEY_TYPE, typename VAL_TYPE>
void PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::clear()
{
    /// sends a command to delete every pairs of key, value
    redisCommand(redis_context, "FLUSHDB");
}


template <typename KEY_TYPE, typename VAL_TYPE>
bool PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::empty()
{
    /// obvious!
    return this->size() == 0;
}


template <typename KEY_TYPE, typename VAL_TYPE>
VAL_TYPE PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::operator[](const KEY_TYPE& key)
{
    return at(key);
}
 

template <typename KEY_TYPE, typename VAL_TYPE>
PseudoUnorderedMap<KEY_TYPE, VAL_TYPE>::~PseudoUnorderedMap()
{
    freeReplyObject(redis_reply);
    redisFree(redis_context);
}

