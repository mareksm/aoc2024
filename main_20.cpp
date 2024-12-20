#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <array>
#include <string>

using namespace std;

using Grid = vector<vector<int>>;
constexpr array<pair<int, int>, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

struct Point
{
    int row, col, time;
    bool operator==(const Point &other) const
    {
        return row == other.row && col == other.col;
    }
};

struct PairHash
{
    size_t operator()(const pair<int, int> &p) const
    {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

/*  BFS, returning the time to reach end if provided */
int walk_and_update_time(Grid &grid, const Point &start, const Point &end = {-1, -1, 0})
{
    unordered_set<pair<int, int>, PairHash> visited;
    queue<Point> q;
    q.push(start);
    int result = 0;

    while (!q.empty())
    {
        Point current = q.front();
        q.pop();

        if (current == end)
        {
            result = current.time;
        }

        visited.insert({current.row, current.col});
        grid[current.row][current.col] = current.time;

        for (const auto &direction : directions)
        {
            int nr = current.row + direction.first;
            int nc = current.col + direction.second;

            if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size() &&
                grid[nr][nc] == 0 && !visited.contains({nr, nc}))
            {
                q.push({nr, nc, current.time + 1});
            }
        }
    }

    return result;
}

int search(const Grid &grid, const Grid &ge, int base, int cheat)
{
    int result = 0;

    for (int r = 1; r < grid.size() - 1; r++)
    {
        for (int c = 1; c < grid[0].size() - 1; c++)
        {
            if (grid[r][c] != -1)
            {
                for (int er = max(1, r - cheat); er <= min((int) grid.size() - 2, r + cheat); er++)
                {
                    int max_ec = c + cheat - abs(r - er);
                    int min_ec = c - cheat + abs(r - er);

                    for (int ec = max(1, min_ec); ec <= min((int) grid[0].size() - 2, max_ec); ec++)
                    {
                        if (grid[er][ec] != -1 && base - (grid[r][c] + abs(er - r) + abs(ec - c) + ge[er][ec]) >= 100)
                        {
                            result++;
                        }
                    }
                }
            }
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    ifstream file("input_d20.txt");
    string line;

    Grid grid;
    Point start, end;

    int r = 0;
    while (getline(file, line))
    {
        vector<int> cur;
        for (int c = 0; c < line.size(); c++)
        {
            if (line[c] == '#')
            {
                cur.push_back(-1);
            }
            else
            {
                if (line[c] == 'S')
                {
                    start = {r, c, 0};
                }
                else if (line[c] == 'E')
                {
                    end = {r, c, 0};
                }
                cur.push_back(0);
            }
        }
        grid.push_back(cur);
        r++;
    }

    Grid ge(grid);

    int b = walk_and_update_time(grid, start, end);
    walk_and_update_time(ge, end);

    cout << "DAY 20 (a): " << search(grid, ge, b, 2) << endl;
    cout << "DAY 20 (b): " << search(grid, ge, b, 20) << endl;

    return 0;
}
