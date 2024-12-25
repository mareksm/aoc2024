#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>

using namespace std;

int main(int argc, char **argv)
{
    ifstream file("input_d25.txt");
    string line;

    int r = 0;
    vector<int> val;
    bool in_lock = false;
    vector<array<int, 5>> locks, keys;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (r == 0)
            in_lock = (line == "#####");

        for (char p : line)
            val.push_back(p == '#');

        if (++r == 7)
        {
            array<int, 5> v = {-1, -1, -1, -1, -1};
            for (int i = 0; i < val.size(); i++)
                v[i % 5] += val[i];

            if (in_lock)
                locks.push_back(v);
            else
                keys.push_back(v);

            val.clear();
            r = 0;
        }
    }

    int day_25_a = 0;
    for (const auto &l : locks)
    {
        for (const auto &k : keys)
        {
            int fit = 0;
            for (int i = 0; i < 5; i++)
            {
                if (l[i] + k[i] <= 5)
                    fit++;
            }

            day_25_a += fit == 5;
        }
    }

    cout << "DAY 25 (a): " << day_25_a << "\n";

    return 0;
}