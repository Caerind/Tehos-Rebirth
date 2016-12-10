#pragma once

#include <functional>
#include <string>

namespace ke
{

inline std::size_t hashType(std::string const& h)
{
	std::hash<std::string> hash_fn;
	return hash_fn(h);
}

} // namespace ke

// TODO : Hash only at compilation

#define TYPE(CLASS_NAME) static std::string getStaticType() { return #CLASS_NAME; } \
						 static std::size_t getStaticTypeId() { return ke::hashType(#CLASS_NAME); } \
                         virtual std::string getType() { return getStaticType(); } \
					     virtual std::size_t getTypeId() { return getStaticTypeId(); }
