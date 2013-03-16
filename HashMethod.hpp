#ifndef HASHMETHOD_H
#define HASHMETHOD_H

#include "md5.h"
#include <string>

/**
  * Hashing method interface.
  */
class HashMethod
{
	protected:
		std::string _name;
		/**
		  * Creates 
		  * name : Name of the method.
		  */
		HashMethod(std::string name) : _name(name) {}
	public:
		/**
		  * Hash a password.
		  * pwd : Password to hash.
		  */
		virtual std::string hash(const std::string& pwd) const=0;
		
		/**
		  * Returns the name of the method.
		  */
		std::string name()
		{
			return _name;
		}
};

/**
  * MD5 hashing method.
  */
class MD5Hash : public HashMethod
{
	public:
		/**
		  * Constructor.
		  */
		MD5Hash() : HashMethod("md5") {}
		
		/**
		  * See HashMethod::hash.
		  */
		virtual std::string hash(const std::string& pwd) const
		{
			return md5(pwd);
		}
};

#endif
