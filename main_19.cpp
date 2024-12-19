#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <string>

using namespace std;

size_t find_helper(const string &input, size_t index, const unordered_set<string> &pset, vector<size_t> &memo)
{
    if (index == input.size())
        return 1;

    if (memo[index] != static_cast<size_t>(-1))
        return memo[index];

    size_t count = 0;
    for (const auto &pattern : pset)
    {
        size_t len = pattern.size();
        if (index + len <= input.size() && input.compare(index, len, pattern) == 0)
        {
            count += find_helper(input, index + len, pset, memo);
        }
    }

    memo[index] = count;
    return count;
}

size_t find(const string &input, const unordered_set<string> &pset)
{
    vector<size_t> memo(input.size(), static_cast<size_t>(-1));
    return find_helper(input, 0, pset, memo);
}

int main()
{
    ifstream file("input_d19.txt");
    string line;
    size_t ln = 0, day_19_a = 0, day_19_b = 0;
    unordered_set<string> pattern;

    while (getline(file, line))
    {
        if (ln++ == 0)
        {
            size_t start = 0, pos = 0;
            while ((pos = line.find(", ", start)) != string::npos)
            {
                pattern.insert(line.substr(start, pos - start));
                start = pos + 2;
            }
            pattern.insert(line.substr(start));
        }
        else if (!line.empty())
        {
            size_t c = find(line, pattern);
            if (c > 0)
            {
                day_19_a++;
                day_19_b += c;
            }
        }
    }

    cout << "DAY 19 (a): " << day_19_a << endl;
    cout << "DAY 19 (b): " << day_19_b << endl;

    return 0;
}
