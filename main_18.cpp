#define _CRT_SECURE_NO_WARNINGS
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
#include <ranges>
#include <cmath>

using namespace std;

#if 1
constexpr auto size_x = 71;
constexpr auto size_y = 71;
constexpr auto filename = "input_d18.txt";
int part1_bytes = 1024;
#else
constexpr auto size_x = 7;
constexpr auto size_y = 7;
constexpr auto filename = "input_d18t.txt";
int part1_bytes = 12;
#endif

using Grid = vector<vector<char>>;

bool valid(int x, int y, const Grid &grid)
{
    int mx = grid.size();
    int my = grid[0].size();
    return (x >= 0 && y >= 0 && x < mx && y < my && grid[x][y] != '#');
}

int find(const Grid &grid)
{
    /* Dijkstra */
    int rows = grid.size();
    int cols = grid[0].size();

    const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    vector<vector<int>> distances(rows, vector<int>(cols, INT_MAX));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    pq.emplace(0, 0, 0); // (distance, x, y)
    distances[0][0] = 0;

    while (!pq.empty())
    {
        auto [dist, x, y] = pq.top();
        pq.pop();

        if (x == rows - 1 && y == cols - 1)
        {
            return dist;
        }

        for (const auto &direction : directions)
        {
            int nx = x + direction.first;
            int ny = y + direction.second;

            if (valid(nx, ny, grid))
            {
                int nd = dist + 1;
                if (nd < distances[nx][ny])
                {
                    distances[nx][ny] = nd;
                    pq.emplace(nd, nx, ny);
                }
            }
        }
    }

    return -1;
}

int main(int argc, char **argv)
{
    FILE *file = fopen(filename, "r");
    int x, y, i = 0;

    Grid grid(size_y, vector<char>(size_x, '.'));

    while (fscanf(file, "%d,%d\n", &x, &y) == 2)
    {
        if (y < size_y && x < size_x)
            grid[y][x] = '#';

        i++;

        int dist = find(grid);

        if (i == part1_bytes)
        {
            cout << "DAY 18 (a): " << dist << endl;
        }

        if (dist == -1)
        {
            cout << "DAY 18 (b): " << x << "," << y << endl;
            break;
        }
    }
    fclose(file);

    return 0;
}
