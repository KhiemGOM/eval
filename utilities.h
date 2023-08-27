//
// Created by DELL on 27/08/2023.
//

#ifndef UTILITIES_H
#define UTILITIES_H
#include <vector>

template<typename T, typename Iter>
static std::vector<T> subVector(Iter start, Iter end)
{
    return std::vector<T>(start, end);
}
template<typename T, typename Iter, typename V>
static void replace(std::vector<T> &v, Iter start, Iter end, const V &replacement)
{
    v.erase(start, end);
    v.insert(start, replacement);
}
template<typename T, typename Iter>
[[maybe_unused]] static void replace(std::vector<T> &v, Iter start, Iter end, const std::vector<T> &replacement)
{
    v.erase(start, end);
    v.insert(start, replacement.begin(), replacement.end());
}
template<typename T, typename P>
static auto to_iterator(const std::vector<T> &v, P val)
{
    auto it = v.begin();
    std::advance(it, std::distance(&*v.begin(), val));
    return it;
}


#endif //UTILITIES_H