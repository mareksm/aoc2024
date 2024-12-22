#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <string>
#include <algorithm>

using namespace std;

inline size_t step(size_t s)
{
    s = (s ^ (s << 6)) & 0xFFFFFF;
    s = (s ^ (s >> 5)) & 0xFFFFFF;
    return (s ^ (s << 11)) & 0xFFFFFF;
}

struct TupleHash
{
    size_t operator()(const tuple<int, int, int, int> &t) const
    {
        size_t hash = std::hash<int>{}(get<0>(t));
        hash = hash_combine(hash, get<1>(t));
        hash = hash_combine(hash, get<2>(t));
        hash = hash_combine(hash, get<3>(t));
        return hash;
    }

private:
    static size_t hash_combine(size_t seed, int value)
    {
        return seed ^ (std::hash<int>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }
};

constexpr size_t num_of_secrets = 2000;

int main()
{
    ifstream file("input_d22.txt");
    string line;

    size_t day_22_a = 0, day_22_b = 0;
    vector<size_t> nums;
    unordered_map<tuple<int, int, int, int>, size_t, TupleHash> day_22_b_values;

    while (getline(file, line))
    {
        size_t n = stoi(line);
        nums.clear();
        nums.reserve(num_of_secrets + 1);
        nums.push_back(n);

        for (int i = 0; i < num_of_secrets; ++i)
        {
            n = step(n);
            nums.push_back(n);
            if (i == num_of_secrets - 1)
                day_22_a += n;
        }

        vector<int> diffs(nums.size() - 1);
        for (size_t i = 1; i < nums.size(); ++i)
        {
            diffs[i - 1] = nums[i] % 10 - nums[i - 1] % 10;
        }

        unordered_set<tuple<int, int, int, int>, TupleHash> seen;
        for (size_t i = 0; i <= diffs.size() - 4; ++i)
        {
            auto pattern = make_tuple(diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]);
            if (seen.insert(pattern).second)
            {
                day_22_b_values[pattern] += nums[i + 4] % 10;
            }
        }
    }

    for (const auto &[key, value] : day_22_b_values)
    {
        day_22_b = max(day_22_b, value);
    }

    cout << "DAY 22 (a): " << day_22_a << "\n";
    cout << "DAY 22 (b): " << day_22_b << "\n";

    return 0;
}
