#ifndef KE_CONTAINER_HPP
#define KE_CONTAINER_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "../Maths/Random.hpp"

namespace ke
{

///
///
///

template <typename K, typename T>
void add(std::map<K,T>& map, K const& key, T const& element);

template <typename K, typename T>
void remove(std::map<K,T>& map, K const& key);

template <typename K, typename T>
void append(std::map<K,T>& map, std::map<K,T> const& omap, bool overrideValue = true);

template <typename K, typename T>
bool contains(std::map<K,T> const& map, K const& key);

// -----------------------------------------------------------

template <typename K, typename T>
void add(std::map<K,T>& map, K const& key, T const& element)
{
    map[key] = element;
}

template <typename K, typename T>
void remove(std::map<K,T>& map, K const& key)
{
    auto itr = map.find(key);
    if (itr != map.end())
    {
        map.erase(itr);
    }
}

template <typename K, typename T>
void append(std::map<K,T>& map, std::map<K,T> const& omap, bool overrideValue)
{
    for (auto itr = omap.begin(); itr != omap.end(); itr++)
    {
        if (overrideValue || !contains(map,itr->first))
        {
            map[itr->first] = itr->second;
        }
    }
}

template <typename K, typename T>
bool contains(std::map<K,T> const& map, K const& key)
{
    return (map.find(key) != map.end());
}


///
///
///


template <typename T>
void sort(std::vector<T>& vector, std::function<bool(T const& a, T const& b)> function);

template <typename T>
void add(std::vector<T>& vector, T const& element);

template <typename T>
void remove(std::vector<T>& vector, T const& element);

template <typename T>
void append(std::vector<T>& vector, std::vector<T> const& ovector);

template <typename T>
void erase(std::vector<T>& vector, std::size_t const& index);

template <typename T>
bool contains(std::vector<T> const& vector, T const& element);

template <typename T>
bool validIndex(std::vector<T> const& vector, std::size_t const& index);

template <typename T>
const T& randomElement(std::vector<T> const& vector);

// --------------------------------------------------------------

template <typename T>
void sort(std::vector<T>& vector, std::function<bool(T const& a, T const& b)> function)
{
    std::sort(vector.begin(),vector.end(),function);
}

template <typename T>
void add(std::vector<T>& vector, T const& element)
{
    vector.push_back(element);
}

template <typename T>
void remove(std::vector<T>& vector, T const& element)
{
    for (auto itr = vector.begin(); itr != vector.end(); itr++)
    {
        if (*itr == element)
        {
            vector.erase(itr);
            return;
        }
    }
}

template <typename T>
void append(std::vector<T>& vector, std::vector<T> const& ovector)
{
    for (auto itr = ovector.begin(); itr != ovector.end(); itr++)
    {
        vector.push_back(*itr);
    }
}

template <typename T>
void erase(std::vector<T>& vector, std::size_t const& index)
{
    if (0 <= index && index < vector.size())
    {
        vector.erase(vector.begin() + index);
    }
}

template <typename T>
bool contains(std::vector<T> const& vector, T const& element)
{
    return std::find(vector.begin(),vector.end(),element) != vector.end();
}

template <typename T>
bool validIndex(std::vector<T> const& vector, std::size_t const& index)
{
    return (0 <= index && index < vector.size());
}

template <typename T>
const T& randomElement(std::vector<T> const& vector)
{
    return vector.at(NMath::random(0, static_cast<int>(vector.size() - 1)));
}

} // namespace ke

#endif // KE_CONTAINER_HPP
