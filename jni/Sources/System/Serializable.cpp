#include "Serializable.hpp"
#include "Serializer.hpp"

namespace ke
{

Serializable::Serializable()
{
}

void Serializable::serialize(Serializer& serializer)
{
}

bool Serializable::deserialize(Serializer& serializer)
{
	return true;
}

} // namespace ke