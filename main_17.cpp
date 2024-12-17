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

vector<int> run(unordered_map<int, int64_t> &regs, vector<int> &prog)
{
    int ip = 0;
    vector<int> out;

    while (ip < prog.size())
    {
        int opc = prog[ip];
        int64_t opr = prog[ip + 1];

        if (opr > 3)
        {
            opr = opr == 4 ? regs['A'] : opr == 5 ? regs['B']
                                                  : regs['C'];
        }

        switch (opc)
        {
        case 0:
            regs['A'] /= pow(2, opr);
            break;
        case 1:
            regs['B'] ^= opr;
            break;
        case 2:
            regs['B'] = opr % 8;
            break;
        case 3:
            if (regs['A'] != 0)
            {
                ip = opr;
                continue;
            }
            break;
        case 4:
            regs['B'] = regs['C'] ^ regs['B'];
            break;
        case 5:
            out.push_back(opr % 8);
            break;
        case 6:
            regs['B'] = regs['A'] / pow(2, opr);
            break;
        case 7:
            regs['C'] = regs['A'] / pow(2, opr);
            break;
        }

        ip = ip + 2;
    }

    return out;
}

int64_t run_reverse(int64_t a, int n, vector<int> &prog)
{
    if (n > prog.size())
        return a;

    for (int i = 0; i < 8; i++)
    {
        int64_t a_next = (a << 3) | i;
        unordered_map<int, int64_t> regs{{'A', a_next}, {'B', 0}, {'C', 0}};
        vector<int> testprog(prog.end() - n, prog.end());

        if (run(regs, prog) == testprog)
        {
            int64_t a_out = run_reverse(a_next, n + 1, prog);
            if (a_out != -1)
                return a_out;
        }
    }
    return -1;
}

int main(int argc, char **argv)
{
    ifstream file("input_d17.txt");
    string line;

    unordered_map<int, int64_t> regs;
    vector<int> prog;

    while (getline(file, line))
    {
        if (line.starts_with("Register "))
        {
            regs.insert({line.at(9), stoi(line.data() + 11)});
        }
        else if (line.starts_with("Program: "))
        {
            ranges::copy(line.substr(9) | ranges::views::split(',') | ranges::views::transform([](auto &&sub)
                                                                                               { return stoi(string(sub.begin(), sub.end())); }),
                         back_inserter(prog));
            break;
        }
    }

    unordered_map<int, int64_t> regs_orig(regs);
    vector<int> out = run(regs, prog);

    cout << "DAY 17 (a): ";
    for (size_t i = 0; i < out.size(); ++i)
    {
        if (i > 0)
            cout << ",";
        cout << out[i];
    }
    cout << endl;

    cout << "DAY 17 (b): " << run_reverse(0, 1, prog) << endl;

    return 0;
}
