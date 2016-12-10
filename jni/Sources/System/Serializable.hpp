#ifndef KE_SERIALIZABLE_HPP
#define KE_SERIALIZABLE_HPP

#include "String.hpp"

namespace ke
{

class Serializer;
class Serializable
{
	public:
		Serializable();

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);
};

} // namespace ke

#endif // KE_SERIALIZABLE_HPP
