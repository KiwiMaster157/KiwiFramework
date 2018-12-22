#pragma once

#include <functional>

namespace kiwi
{
namespace utility
{
inline namespace v1
{

template<typename T>
using Predicate = std::function<bool(const T&)>;

template<typename T>
Predicate<T> isEqual(const T& key)
{
	return [=](const T& k)->bool { return k == key; };
}

template<typename T>
Predicate<T> isNotEqual(const T& key)
{
	return [=](const T& k)->bool { return k != key; };
}

template<typename T>
Predicate<T> isLess(const T& key)
{
	return [=](const T& k)->bool {return k < key; };
}

template<typename T>
Predicate<T> isNotLess(const T& key)
{
	return [=](const T& k)->bool {return k >= key; };
}

template<typename T>
Predicate<T> isGreater(const T& key)
{
	return [=](const T& k)->bool {return k > key; };
}

template<typename T>
Predicate<T> isNotGreater(const T& key)
{
	return [=](const T& k)->bool {return k <= key; };
}

using Mask = unsigned long long;

inline Predicate<Mask> isMasked(const Mask& key)
{
	return [=](const Mask& k)->bool {return k & key; };
}

inline Predicate<Mask> isNotMasked(const Mask& key)
{
	return [=](const Mask& k)->bool {return !(k & key); };
}

inline Predicate<Mask> isUnmasked(const Mask& key)
{
	return [=](const Mask& k)->bool {return k & ~key; };
}

inline Predicate<Mask> isNotUnmasked(const Mask& key)
{
	return [=](const Mask& k)->bool {return !(k & ~key); };
}

}
}
}