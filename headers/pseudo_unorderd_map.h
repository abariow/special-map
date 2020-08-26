#ifndef PSEUDOUNORDEREDMAP_H
#define PSEUDOUNORDEREDMAP_H
 
#include <string>
#include <hiredis/hiredis.h>


class CouldNotConnectToRedisException
{
    public:
        const std::string error_message = "Couldn't connect to redis server, try again";
};


class KeyNotFoundException
{
    public:
        explicit KeyNotFoundException(const std::string& key) 
        {
            error_message = "Key '" + key  + "' not found in database.";
        }

        std::string error_message;
};


class EmptyStringException
{
    public:
        const std::string error_message = "Empty key or value";
};



enum ConnectionType 
{
    TCP, UNIX_SOCKET, SHARED_MEMORY
};


template <typename KEY_TYPE, typename VAL_TYPE>
class PseudoUnorderedMap
{
 
public:
	
    ///< constructors:

    /*****************************************************************//**
    * takes ip address and port of redis server as arguments. 
    *********************************************************************/
    PseudoUnorderedMap(const std::string& ip_address, unsigned int port);


    /*****************************************************************//**
    * takes socket path and connection type (Unix or Shared Memory)
    * as arguments, throws an exception if gets TCP as Connection Type. 
    *********************************************************************/
    PseudoUnorderedMap(const std::string& socket_path, ConnectionType ct);

    
    ///< member functions:

    /*****************************************************************//**
    * takes a key as argument, then returns the corresponding value.
    *********************************************************************/
    VAL_TYPE at(const KEY_TYPE& key);

    /*****************************************************************//**
    * takes a pair of key, value, then push it in database.
    *********************************************************************/
    void insert(const KEY_TYPE& key, const VAL_TYPE& value);
    
    /*****************************************************************//**
    * returns the No. of key,value pairs.
    *********************************************************************/
    size_t size();

    /*****************************************************************//**
    * removes all pairs in database.
    *********************************************************************/
    void clear();

    /*****************************************************************//**
    * returns whether database is empty or not.
    *********************************************************************/
    bool empty();

    /*****************************************************************//**
    * takes a key as argument, then removes the key and its value from
    * database.
    **********************************************************************/
    bool remove(const KEY_TYPE& key);
    
    ///< operators:
    
    /****************************************************************//**
    * takes a key in bracket, then returns the corresponding value.
    *********************************************************************/
    VAL_TYPE operator[](const KEY_TYPE& key);


    ///< destructor: 
    ~PseudoUnorderedMap();
 
    
private:
    
    redisContext *redis_context; //!< to hold connection state 
    redisReply *redis_reply; //!< to hold redis replies
           
    
};

#include "pseudo_unorderd_map.inl"

#endif

