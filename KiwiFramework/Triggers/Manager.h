#pragma once

#include <list>
#include <memory>
#include <utility>
#include <functional>

namespace kiwi
{
inline namespace v1
{

//Note that ValueType may be an abstract type. Therefore, 
template<typename KeyType, typename ValueType>
class Manager
{
public:
	using Key = KeyType;
	using Value = ValueType;
	using Pred = std::function<bool(const Key&)>;
	using Size = std::size_t;
	using Agent = std::function<void(Value&)>;

	using ValuePtr = std::unique_ptr<Value>;
	using Pair = std::pair<Key, ValuePtr>;
	using Container = std::list<Pair>;

	Manager() = default;
	Manager(const Manager&) = delete;
	Manager(Manager&&) = default;

	virtual ~Manager() = default;

	//place >= 0: position from beginning
	//place < 0: position from end
	void insert(Key key, const Value& val, int place = -1)
	{
		if (place >= 0)
		{
			typename Container::iterator i = data.begin();
			for (; place > 0; place--)
				i++;
			data.insert(i, std::make_pair(key, std::make_unique(val)));
		}
		else
		{
			typename Container::iterator i = data.end();
			for (place++; place < 0; place++)
				i--;
			data.insert(i, std::make_pair(key, std::make_unique(val)));
		}
	}

	/* TO BE IMPLEMENTED PROPERLY ONCE A NEED IS SHOWN
	void insert(Key key, Value&& val)
	{
		data.push_back(std::make_pair(key, std::make_unique(std::move(val))));
	}*/

	Size remove(const Key& key)
	{
		return this->remove([&](const Key& k)->bool { return k == key; });
	}

	Size remove(const Pred& pred)
	{
		Size count = 0;

		for (auto i = data.begin(); i != data.end(); i++)
		{
			if (pred(i->first))
			{
				i++;
			}
			else
			{
				i = data.remove(i);
				count++;
			}
		}

		return count;
	}

	void clear()
	{
		data.clear();
	}

	Size count(const Key& key) const
	{
		return this->count([&](const Key& k)->bool { return k == key; });
	}

	Size count(const Pred& pred) const
	{
		Size count = 0;

		for (const auto& el : data)
		{
			if (pred(el.first))
			{
				count++;
			}
		}

		return count;
	}

	Size count() const
	{
		return data.size();
	}

	void sort()
	{
		data.sort([](const Pair& lhs, const Pair& rhs)->bool
			{
				return lhs.first < rhs.first;
			});
	}

	void operator()(const Key& key, Agent fn)
	{
		(*this)([&](const Key& k)->bool { return k == key; }, fn);
	}

	void operator()(const Pred& pred, Agent fn)
	{
		for (auto& el : data)
		{
			if (pred(el.first))
			{
				fn(*el.second);
			}
		}
	}

	void operator()(Agent fn)
	{
		for (auto& el : data)
		{
			fn(*el.second);
		}
	}

private:
	Container data; //Pair = std::pair<Key, ValuePtr>
};

}
}