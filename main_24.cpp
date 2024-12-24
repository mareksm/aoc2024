#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <tuple>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <set>

using namespace std;

int evaluate(const string &op, unordered_map<string, tuple<string, string, string>> &gates, unordered_map<string, int> &values)
{
    if (values.contains(op))
        return values[op];

    const auto &[op1, g, op2] = gates[op];
    int a = evaluate(op1, gates, values);
    int b = evaluate(op2, gates, values);
    int r = 0;

    if (g == "AND")
    {
        r = a & b;
    }
    else if (g == "OR")
    {
        r = a | b;
    }
    else if (g == "XOR")
    {
        r = a ^ b;
    }

    return r;
}

string findGate(const string &a, const string &b, const string &op, const vector<string> &gates)
{
    for (const auto &gate : gates)
    {
        if (gate.find(a + " " + op + " " + b) == 0 || gate.find(b + " " + op + " " + a) == 0)
        {
            return gate.substr(gate.find(" -> ") + 4);
        }
    }
    return "";
}

void evaluate2(vector<string> &data)
{
    set<string> swapped;
    string c0;

    for (int i = 0; i < 45; i++)
    {
        string n = (i < 10 ? "0" : "") + to_string(i);
        string i_sum = findGate("x" + n, "y" + n, "XOR", data);
        string i_carry = findGate("x" + n, "y" + n, "AND", data);
        string r1, f_sum, f_carry;

        if (!c0.empty())
        {
            r1 = findGate(c0, i_sum, "AND", data);

            if (r1.empty())
            {
                swap(i_sum, i_carry);
                swapped.insert(i_sum);
                swapped.insert(i_carry);
                r1 = findGate(c0, i_sum, "AND", data);
            }

            f_sum = findGate(c0, i_sum, "XOR", data);

            if (i_sum.find("z") == 0)
            {
                swap(i_sum, f_sum);
                swapped.insert(i_sum);
                swapped.insert(f_sum);
            }

            if (i_carry.find("z") == 0)
            {
                swap(i_carry, f_sum);
                swapped.insert(i_carry);
                swapped.insert(f_sum);
            }

            if (r1.find("z") == 0)
            {
                swap(r1, f_sum);
                swapped.insert(r1);
                swapped.insert(f_sum);
            }

            f_carry = findGate(r1, i_carry, "OR", data);
        }

        if (!f_carry.empty() && f_carry.find("z") == 0 && f_carry != "z45")
        {
            swap(f_carry, f_sum);
            swapped.insert(f_carry);
            swapped.insert(f_sum);
        }

        c0 = f_carry.empty() ? i_carry : f_carry;
    }

    cout << "DAY 24 (b): ";
    for (auto it = swapped.begin(); it != swapped.end(); it++)
    {
        cout << *it;
        if (next(it) != swapped.end())
            cout << ",";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    ifstream file("input_d24.txt");
    string line;
    bool isGates = false;

    vector<string> data2;
    unordered_map<string, tuple<string, string, string>> gates;
    unordered_map<string, int> values;

    while (getline(file, line))
    {
        if (line.empty())
        {
            isGates = true;
            continue;
        }

        if (isGates)
        {
            string a, op, b, arrow, r;
            istringstream iss(line);
            iss >> a >> op >> b >> arrow >> r;
            gates[r] = {a, op, b};
            data2.push_back(line);
        }
        else
        {
            string op;
            int v;
            istringstream iss(line);
            iss >> op >> v;
            values[op.substr(0, op.size() - 1)] = v;
        }
    }

    bitset<64> bitset;

    for (const auto &[r, _] : gates)
    {
        if (r[0] == 'z')
        {
            bitset[stoi(r.substr(1))] = evaluate(r, gates, values);
        }
    }

    cout << "DAY 24 (a): " << bitset.to_ullong() << endl;

    evaluate2(data2);

    return 0;
}