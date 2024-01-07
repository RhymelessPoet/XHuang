#pragma once
#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <list>
#include <forward_list>
#include <tuple>
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

template <typename T>
using Set = std::set<T>;
template <typename T>
using MultiSet = std::multiset<T>;
template <typename T>
using UnorderedSet = std::unordered_set<T>;
template <typename T>
using UnorderedMultiSet = std::unordered_multiset<T>;

template <typename Head, typename ...Tail>
using Tuple = std::tuple<Head, Tail...>;

} // namespace XHuang