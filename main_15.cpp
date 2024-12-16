#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

using Robot = pair<int, int>;
using Grid = vector<vector<char>>;

const unordered_map<char, pair<int, int>> dirs = {
    {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}, {'^', {-1, 0}}};

struct pair_hash
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        return hash<T1>{}(p.first) ^ (hash<T2>{}(p.second) << 1);
    }
};

void parse_input(const string &filename, vector<char> &moves, Grid &grid, Robot &pos)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (string directions = "<v^>"; directions.find(line[0]) != string::npos)
        {
            moves.insert(moves.end(), line.begin(), line.end());
        }
        else
        {
            vector<char> row;
            for (size_t i = 0; i < line.size(); ++i)
            {
                if (line[i] == '@')
                    pos = {grid.size(), static_cast<int>(i)};
                row.push_back(line[i]);
            }
            grid.push_back(row);
        }
    }
}

size_t calculate_result(const Grid &grid, char target)
{
    size_t result = 0;
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
            if (grid[i][j] == target)
                result += 100 * i + j;
    }
    return result;
}

size_t solve_part_a(Grid grid, const vector<char> &moves, Robot pos)
{
    for (char m : moves)
    {
        auto [dr, dc] = dirs.at(m);
        auto [nr, nc] = pair{pos.first + dr, pos.second + dc};

        if (grid[nr][nc] == '#')
            continue;

        if (grid[nr][nc] == '.')
        {
            swap(grid[nr][nc], grid[pos.first][pos.second]);
            pos = {nr, nc};
        }
        else if (grid[nr][nc] == 'O')
        {
            auto [tr, tc] = pair{nr, nc};
            while (grid[tr][tc] == 'O')
            {
                tr += dr;
                tc += dc;
            }
            if (grid[tr][tc] != '#')
            {
                grid[tr][tc] = 'O';
                grid[nr][nc] = '@';
                grid[pos.first][pos.second] = '.';
                pos = {nr, nc};
            }
        }
    }

    return calculate_result(grid, 'O');
}

size_t solve_part_b(Grid grid, const vector<char> &moves, Robot pos)
{
    const unordered_map<char, string> emap = {
        {'#', "##"}, {'O', "[]"}, {'.', ".."}, {'@', "@."}};

    Grid expanded_grid;
    for (const auto &row : grid)
    {
        vector<char> expanded_row;
        for (char cell : row)
            for (char expanded : emap.at(cell))
                expanded_row.push_back(expanded);
        expanded_grid.push_back(expanded_row);
    }

    grid = std::move(expanded_grid);
    pos.second *= 2;

    for (char m : moves)
    {
        auto [rr, rc] = pos;
        auto [dr, dc] = dirs.at(m);
        auto [nr, nc] = pair{rr + dr, rc + dc};

        if (grid[nr][nc] == '.')
        {
            grid[rr][rc] = '.';
            grid[nr][nc] = '@';
            pos = {nr, nc};
            continue;
        }
        else if (grid[nr][nc] == '#')
            continue;

        if (dr == 0)
        {
            auto [tr, tc] = pair{nr, nc};
            int ds = 0;

            while (grid[tr][tc] == '[' || grid[tr][tc] == ']')
            {
                ++ds;
                tr += dr;
                tc += dc;
            }

            if (grid[tr][tc] == '#')
                continue;

            for (int i = 0; i < ds; ++i)
            {
                grid[tr][tc] = grid[tr - dr][tc - dc];
                tr -= dr;
                tc -= dc;
            }
            
            grid[nr][nc] = '@';
            grid[rr][rc] = '.';
            pos = {nr, nc};
            continue;
        }

        vector<unordered_set<pair<int, int>, pair_hash>> tp = {{{rr, rc}}};
        bool nw = true, ae = false;
        while (nw && !ae)
        {
            unordered_set<pair<int, int>, pair_hash> np;
            ae = true;
            for (const auto &[cr, cc] : tp.back())
            {
                if (grid[cr][cc] == '.')
                    continue;

                auto [tr, tc] = pair{cr + dr, cc + dc};
                if (grid[tr][tc] != '.')
                    ae = false;

                np.insert({tr, tc});

                if (grid[tr][tc] == '#')
                {
                    nw = false;
                    break;
                }
                else if (grid[tr][tc] == '[')
                {
                    np.insert({tr, tc + 1});
                }
                else if (grid[tr][tc] == ']')
                {
                    np.insert({tr, tc - 1});
                }
            }
            tp.push_back(np);
        }

        if (!nw)
            continue;

        for (int i = tp.size() - 1; i > 0; --i)
        {
            for (const auto &[cr, cc] : tp[i])
            {
                auto [fr, fc] = pair{cr - dr, cc - dc};
                if (tp[i - 1].contains({fr, fc}))
                    grid[cr][cc] = grid[fr][fc];
                else
                    grid[cr][cc] = '.';
            }
        }

        grid[rr][rc] = '.';
        pos = {nr, nc};
    }

    return calculate_result(grid, '[');
}

int main(int arg, char **argv)
{
    vector<char> moves;
    Grid grid;
    Robot pos = {-1, -1};

    parse_input("input_d15.txt", moves, grid, pos);

    cout << "DAY 15 (a): " << solve_part_a(grid, moves, pos) << endl;
    cout << "DAY 15 (b): " << solve_part_b(grid, moves, pos) << endl;

    return 0;
}
