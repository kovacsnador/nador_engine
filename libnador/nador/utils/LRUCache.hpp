#ifndef __NADOR_UTILS_LRU_CACHE_HPP__
#define __NADOR_UTILS_LRU_CACHE_HPP__

#include <list>
#include <unordered_map>
#include <stdexcept>

namespace nador
{
    struct LRUCacheException : public std::runtime_error
    {
        LRUCacheException(const std::string& msg)
        : std::runtime_error(msg)
        {
        }
    };

    template <typename KeyTy, typename ValueTy>
    class LRUCache
    {
        using list_t              = std::list<std::pair<KeyTy, ValueTy>>;
        using list_iter_t         = list_t::iterator;
        using map_t               = std::unordered_map<KeyTy, list_iter_t>;

    public:
        using onEvictedCallback_t = std::function<void(const KeyTy&, const ValueTy&)>;

        // enable move operations
        LRUCache(LRUCache&&) = default;
        LRUCache& operator=(LRUCache&&) = default;

        // delete copy operations
        LRUCache(const LRUCache&) = delete;
        LRUCache& operator=(const LRUCache&) = delete;

        LRUCache(size_t maxSize, onEvictedCallback_t onEvict = {})
        : _maxSize(maxSize)
        , _onEvict(onEvict)
        {
            if(maxSize == 0)
            {
                throw LRUCacheException{"LRUChache: maxSize needs to be > 0"};
            }
        }

        template <typename T>
        bool Insert(const KeyTy& key, T&& value)
        {
            // check if it's in map
            auto it = _map.find(key);
            if (it == _map.end())
            {
                // Insert new element
                _list.push_front({key, std::forward<T>(value)});
                _map.insert({ key, _list.begin() });

                // Remove least used elements
                while(_list.size() > _maxSize)
                {
                    const auto& [k, _] = *std::prev(_list.end(), 1);
                    Erase(k);
                }
                return true;
            }
            return false;
        }

        template <typename T>
        bool InsertOrAssign(const KeyTy& key, T&& value)
        {
            // Check simple insert
            if (Insert(key, value) == false)
            {
                auto it = _map.find(key);
                if (it != _map.end())
                {
                    // Override value and pull it the front
                    auto& [_, v] = *it->second;
                    v = std::forward<T>(value);
                    _list.splice(_list.begin(), _list, it->second);
                }
                else
                {
                    // Should never happen
                    return false;
                }
            }
            return true;
        }

        ValueTy& Get(const KeyTy& key)
        {
            auto it = _map.find(key);
            if (it == _map.end())
            {
                throw LRUCacheException{ "Element is not in LRUCache." };
            }

            auto& [_, val] = *it->second;
            // Push value to the front if already exist in list
            _list.splice(_list.begin(), _list, it->second);

            return val;
        }

        bool Erase(const KeyTy& key)
        {
            auto it = _map.find(key);
            if (it != _map.end())
            {
                if(_onEvict)
                {
                    const auto& [_, val] = *it->second;
                    _onEvict(key, val);
                }

                auto tempIter = it->second;

                _map.erase(key);
                _list.erase(tempIter);

                return true;
            }
            return false;
        }

        bool Exist(const KeyTy& key) const noexcept
        {
            return _map.find(key) != _map.end();
        }

        size_t Size() const noexcept
        {
            return _list.size();
        }

        void SetonEvictedCallback(onEvictedCallback_t cb)
        {
            _onEvict = cb;
        }

    private:
        size_t _maxSize { 1 };

        list_t _list;
        map_t  _map;

        onEvictedCallback_t _onEvict { nullptr };
    };

} // namespace nador

#endif //!__NADOR_UTILS_LRU_CACHE_HPP__