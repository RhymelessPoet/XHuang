#pragma once
#include <array>
#include <vector>
#include <map>
#include <list>
#include <forward_list>
#include <memory>

namespace XHuang
{

template <typename T, size_t Size>
using Array = std::array<T, Size>;

template <typename T>
using Vector = std::vector<T>;

template <typename Key, typename Value>
using Map = std::map<Key, Value>;
template <typename Key, typename Value>
using MultiMap = std::multimap<Key, Value>;

template <typename T>
using List = std::list<T>;
template <typename T>
using ForwardList = std::forward_list<T>;

} // namespace XHuang