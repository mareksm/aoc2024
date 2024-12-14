#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

#if 1
constexpr auto size_x = 101;
constexpr auto size_y = 103;
constexpr auto filename = "input_d14.txt";
#else
constexpr auto size_x = 11;
constexpr auto size_y = 7;
constexpr auto filename = "input_d14t.txt";
#endif

typedef tuple<int, int, int, int> Robot;

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

void print_grid(vector<Robot> &robots)
{
    vector<vector<char>> matrix(size_y, vector<char>(size_x, '.'));

    for (auto &p : robots)
    {
        int x = get<0>(p);
        int y = get<1>(p);
        if (x >= 0 && x < size_x && y >= 0 && y < size_y)
            matrix[y][x] = 'x';
    }

    for (const auto &row : matrix)
    {
        for (const auto &elem : row)
            cout << elem << ' ';
        cout << endl;
    }
}

bool check_grid(vector<Robot> &robots) /* Easter Egg: all robot positions should be unique? */
{
    unordered_set<pair<int, int>, pair_hash> unique;

    for (auto &p : robots)
    {
        int x = get<0>(p);
        int y = get<1>(p);
        if (unique.count({x, y}))
            return false;
        unique.insert({x, y});
    }

    return true;
}

int main(int argc, char **argv)
{
    FILE *file = fopen(filename, "r");
    int p[2], v[2];

    vector<Robot> robots, robots2;

    while (fscanf(file, "p=%d,%d v=%d,%d\n", &p[0], &p[1], &v[0], &v[1]) == 4)
    {
        robots.push_back({p[0], p[1], v[0], v[1]});
        robots2.push_back({p[0], p[1], v[0], v[1]});
    }
    fclose(file);

    int i = 100;
    while (i-- > 0)
    {
        for (auto &p : robots)
        {
            int x = get<0>(p);
            int y = get<1>(p);
            int vx = get<2>(p);
            int vy = get<3>(p);

            x = (x + vx) % size_x;
            if (x < 0)
                x = size_x + x;
            y = (y + vy) % size_y;
            if (y < 0)
                y = size_y + y;

            get<0>(p) = x;
            get<1>(p) = y;
        }
    }

    int q[4] = {0, 0, 0, 0};
    int size_x_half = size_x / 2;
    int size_y_half = size_y / 2;

    for (auto &p : robots)
    {
        int x = get<0>(p);
        int y = get<1>(p);

        if (x == size_x_half || y == size_y_half)
            continue;

        if (x < size_x_half && y < size_y_half)
            q[0]++;
        if (x > size_x_half && y < size_y_half)
            q[1]++;
        if (x < size_x_half && y > size_y_half)
            q[2]++;
        if (x > size_x_half && y > size_y_half)
            q[3]++;
    }

    cout << "DAY 14 (a): " << q[0] * q[1] * q[2] * q[3] << endl;

    i = 10000;
    while (i-- > 0)
    {
        for (auto &p : robots2)
        {
            int x = get<0>(p);
            int y = get<1>(p);
            int vx = get<2>(p);
            int vy = get<3>(p);

            x = (x + vx) % size_x;
            if (x < 0)
                x = size_x + x;
            y = (y + vy) % size_y;
            if (y < 0)
                y = size_y + y;

            get<0>(p) = x;
            get<1>(p) = y;
        }

        if (check_grid(robots2))
        {
            print_grid(robots2);
            cout << "DAY 14 (b): " << 10000 - i << endl;
            break;
        }
    }

    return 0;
}

// g++ -o t main_14.cpp -std=gnu++23