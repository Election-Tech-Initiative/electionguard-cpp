#include <exception>
#include <map>

using std::map;
using std::out_of_range;

template <typename K, typename V> K findByValue(const map<K, const V> &collection, const V &value)
{
    for (auto &element : collection) {
        if (element.second == value) {
            return element.first;
        }
    }
    throw out_of_range("value not found");
}
