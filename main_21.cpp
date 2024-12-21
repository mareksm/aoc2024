#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <limits>
#include <functional>
#include <queue>
#include <string>

using namespace std;

const unordered_map<char, pair<int, int>> positions = {
    {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}}, {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}}, {'0', {3, 1}}, {'A', {3, 2}}, {'^', {0, 1}}, {'a', {0, 2}}, {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};

const unordered_map<char, pair<int, int>> directions = {
    {'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};

struct tuple_hash
{
    template <typename T1, typename T2, typename T3>
    size_t operator()(const tuple<T1, T2, T3> &t) const
    {
        size_t h1 = hash<T1>{}(get<0>(t));
        size_t h2 = hash<T2>{}(get<1>(t));
        size_t h3 = hash<T3>{}(get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

unordered_map<tuple<string, int, int>, size_t, tuple_hash> memos;

vector<string> generate_moveset(pair<int, int> start, pair<int, int> end, pair<int, int> avoid = {0, 0})
{
    auto [dx, dy] = pair{end.first - start.first, end.second - start.second};
    string move_str;
    move_str += string(abs(dx), dx < 0 ? '^' : 'v');
    move_str += string(abs(dy), dy < 0 ? '<' : '>');

    vector<string> permutations;
    sort(move_str.begin(), move_str.end());

    do
    {
        permutations.push_back(move_str);
    } while (next_permutation(move_str.begin(), move_str.end()));

    vector<string> res;
    for (const auto &p : permutations)
    {
        bool valid = true;
        pair<int, int> pos = start;
        for (char dir : p)
        {
            pos.first += directions.at(dir).first;
            pos.second += directions.at(dir).second;
            if (pos == avoid)
            {
                valid = false;
                break;
            }
        }

        if (valid)
            res.push_back(p + 'a');
    }

    if (res.empty())
        return {"a"};

    return res;
}

size_t length(const string &str, int lim, int depth = 0)
{
    auto k = make_tuple(str, depth, lim);
    if (memos.contains(k))
        return memos[k];

    auto avoid = !depth ? make_pair(3, 0) : make_pair(0, 0);
    auto cur = !depth ? positions.at('A') : positions.at('a');
    size_t len = 0;

    for (char ch : str)
    {
        auto next_cur = positions.at(ch);
        auto moveset = generate_moveset(cur, next_cur, avoid);

        if (depth == lim)
        {
            len += moveset[0].size();
        }
        else
        {
            size_t min_len = numeric_limits<size_t>::max();
            for (const auto &m : moveset)
                min_len = min(min_len, length(m, lim, depth + 1));
            len += min_len;
        }
        cur = next_cur;
    }

    memos[k] = len;
    return len;
}

int main(int argc, char **argv)
{
    ifstream file("input_d21.txt");
    string line;

    size_t day_21_a = 0, day_21_b = 0;

    while (getline(file, line))
    {
        size_t len_a = length(line, 2);
        size_t len_b = length(line, 25);
        int num = stoi(line.substr(0, 3));

        day_21_a += len_a * num;
        day_21_b += len_b * num;
    }

    cout << "DAY 21 (a): " << day_21_a << "\n";
    cout << "DAY 21 (b): " << day_21_b << "\n";

    return 0;
}
