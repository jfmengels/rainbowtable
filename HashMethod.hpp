#include "md5.h"
#include <string>

class HashMethod {
	protected:
		std::string _name;
		HashMethod(std::string name) : _name(name) {}
	public:
		virtual std::string hash(const std::string& pwd) const=0;
		std::string name() {
			return _name;
		}
};

class MD5Hash : public HashMethod {
	public:
		MD5Hash() : HashMethod("md5") {}
		virtual std::string hash(const std::string& pwd) const {
			return md5(pwd);
		}
};
