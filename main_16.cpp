#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <limits>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

using Point = pair<int, int>;
using Grid = vector<vector<char>>;

const vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

struct TupleHash
{
    template <class T1, class T2, class T3>
    size_t operator()(const tuple<T1, T2, T3> &t) const
    {
        auto [a, b, c] = t;
        return hash<T1>()(a) ^ hash<T2>()(b) ^ hash<T3>()(c);
    }
};

struct PointHash
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        return hash<T1>{}(p.first) ^ (hash<T2>{}(p.second) << 1);
    }
};

size_t walk(const Grid &grid, Point start, Point end, bool part2)
{
    /* Dijkstra */
    unordered_map<tuple<int, int, int>, vector<tuple<int, int, int>>, TupleHash> prev;
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> pq;
    unordered_map<tuple<int, int, int>, int, TupleHash> dist;
    const auto rows = grid.size(), cols = grid[0].size();

    pq.emplace(0, start.first, start.second, 0);
    dist[{start.first, start.second, 0}] = 0;

    while (!pq.empty())
    {
        auto [d, r, c, dir] = pq.top();
        pq.pop();

        if (grid[r][c] == 'E')
        {
            if (!part2)
                return d;

            unordered_set<Point, PointHash> unique;
            queue<tuple<int, int, int>> q{{{r, c, 0}, {r, c, 1}, {r, c, 2}, {r, c, 3}}};

            while (!q.empty())
            {
                auto [cr, cc, cdir] = q.front();
                q.pop();
                unique.emplace(cr, cc);
                for (const auto &pred : prev[{cr, cc, cdir}])
                    q.push(pred);
            }
            return unique.size();
        }

        for (int i = 0; i < directions.size(); i++)
        {
            int nr = r + directions[i].first;
            int nc = c + directions[i].second;
            int nd = d + 1;

            if (i != dir)
                nd += 1000;

            if (nr < 0 || nc < 0 || nr >= rows || nc >= cols)
                continue;

            if (grid[nr][nc] == '#')
                continue;

            auto current = make_tuple(nr, nc, i);
            if (dist.find(current) == dist.end() || nd < dist[current])
            {
                dist[current] = nd;
                pq.emplace(nd, nr, nc, i);
                prev[current].clear();
                prev[current].emplace_back(r, c, dir);
            }
            else if (nd == dist[current])
            {
                prev[current].emplace_back(r, c, dir);
            }
        }
    }

    return -1;
}

int main(int argc, char **argv)
{
    ifstream file("input_d16.txt");
    string line;

    Grid grid;
    Point start, end;

    while (getline(file, line))
    {
        vector<char> row;
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] == 'S')
                start = {grid.size(), i};
            else if (line[i] == 'E')
                end = {grid.size(), i};

            row.push_back(line[i]);
        }
        grid.push_back(row);
    }

    cout << "DAY 16 (a): " << walk(grid, start, end, false) << endl;
    cout << "DAY 16 (b): " << walk(grid, start, end, true) << endl;

    return 0;
}
