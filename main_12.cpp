#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>

using namespace std;

const int SIZE = 140;
const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

int main(int argc, char **argv)
{
    ifstream file("input_d12.txt");
    string line;
    char grid[SIZE][SIZE];
    int gridsz = 0;

    while (getline(file, line))
    {
        for (int i = 0; i < line.size(); i++)
        {
            grid[gridsz][i] = line[i];
        }
        gridsz++;
    }

    unordered_set<pair<int, int>, pair_hash> visited;
    int day_12_a = 0, day_12_b = 0;

    for (int r = 0; r < gridsz; r++)
    {
        for (int c = 0; c < gridsz; c++)
        {
            if (visited.count({r, c}))
                continue;

            queue<pair<int, int>> q;
            q.push({r, c});
            int area = 0, perim = 0, sides = 0;
            unordered_map<pair<int, int>, unordered_set<pair<int, int>, pair_hash>, pair_hash> perimeter;

            while (!q.empty())
            {
                auto e = q.front();
                q.pop();

                if (visited.count(e))
                    continue;

                visited.insert(e);
                area++;

                for (auto &d : dirs)
                {
                    int rr = e.first + d[0];
                    int cc = e.second + d[1];

                    if (rr >= 0 && rr < gridsz && cc >= 0 && cc < gridsz && grid[rr][cc] == grid[e.first][e.second])
                    {
                        q.push({rr, cc});
                    }
                    else
                    {
                        perim++;
                        perimeter[{d[0], d[1]}].insert(e);
                    }
                }
            }

            for (auto &[_, v] : perimeter)
            {
                unordered_set<pair<int, int>, pair_hash> visited_perimeter;

                for (auto &e : v)
                {
                    if (visited_perimeter.count(e))
                        continue;

                    sides++;
                    queue<pair<int, int>> q;
                    q.push(e);

                    while (!q.empty())
                    {
                        auto f = q.front();
                        q.pop();

                        if (visited_perimeter.count(f))
                            continue;

                        visited_perimeter.insert(f);

                        for (auto &d : dirs)
                        {
                            int rr = f.first + d[0];
                            int cc = f.second + d[1];
                            if (v.count({rr, cc}))
                            {
                                q.push({rr, cc});
                            }
                        }
                    }
                }
            }

            day_12_a += area * perim;
            day_12_b += area * sides;
        }
    }

    cout << "DAY 12 (a): " << day_12_a << "\n";
    cout << "DAY 12 (b): " << day_12_b << "\n";

    return 0;
}

// g++ -o t main.cpp -std=gnu++23