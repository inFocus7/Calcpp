#ifndef XTRAFUNCTIONS_H
#define XTRAFUNCTIONS_H

namespace xtra {

template <typename T, typename T2>
bool is_in(const T& data, const std::initializer_list<T2>& list)
{
    for(const auto& i : list)
    {
        if (data == i)
        {
            return true;
        }
    }
    return false;
}
}
#endif // XTRAFUNCTIONS_H
