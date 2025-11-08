#pragma once

#include <string>

struct User
{
    std::string name;
    int rank{};

    bool operator<(const User& other) const
    {
        return rank < other.rank;
    }

    bool operator>(const User& other) const
    {
        return rank > other.rank;
    }
};