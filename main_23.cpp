#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <tuple>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <iterator>

using namespace std;

int part_1(const map<string, set<string>> &net)
{
    int triplet_count = 0;

    for (const auto &[node, neighbors] : net)
    {
        if (node[0] == 't')
        {
            for (const auto &neighbor1 : neighbors)
            {
                for (const auto &neighbor2 : neighbors)
                {
                    if (neighbor1 != neighbor2 && net.at(neighbor1).count(neighbor2))
                    {
                        triplet_count++;
                    }
                }
            }
        }
        else
        {
            for (const auto &neighbor1 : neighbors)
            {
                if (neighbor1[0] == 't')
                {
                    for (const auto &neighbor2 : neighbors)
                    {
                        if (neighbor1 != neighbor2 && net.at(neighbor1).count(neighbor2))
                        {
                            triplet_count++;
                        }
                    }
                }
                else
                {
                    for (const auto &neighbor2 : neighbors)
                    {
                        if (neighbor2[0] == 't' && neighbor1 != neighbor2 && net.at(neighbor1).count(neighbor2))
                        {
                            triplet_count++;
                        }
                    }
                }
            }
        }
    }

    return triplet_count / 6; // each triplet is counted 6 times, so divide by 6
}

void bron_kerbosch(set<string> R, set<string> P, set<string> X,
                   const map<string, set<string>> &G, vector<set<string>> &cliques)
{
    if (P.empty() && X.empty())
    {
        cliques.push_back(R);
        return;
    }

    while (!P.empty())
    {
        auto it = P.begin();
        string v = *it;
        set<string> R_new = R;
        R_new.insert(v);

        set<string> P_new, X_new;
        set_intersection(P.begin(), P.end(), G.at(v).begin(), G.at(v).end(), inserter(P_new, P_new.begin()));
        set_intersection(X.begin(), X.end(), G.at(v).begin(), G.at(v).end(), inserter(X_new, X_new.begin()));

        bron_kerbosch(R_new, P_new, X_new, G, cliques);

        P.erase(it);
        X.insert(v);
    }
}

void part_2(const map<string, set<string>> &G)
{
    set<string> R, P, X;
    vector<set<string>> cliques;

    for (const auto &[node, _] : G)
    {
        P.insert(node);
    }

    bron_kerbosch(R, P, X, G, cliques);

    const auto &largest_clique = *max_element(cliques.begin(), cliques.end(),
                                              [](const set<string> &a, const set<string> &b)
                                              {
                                                  return a.size() < b.size();
                                              });

    cout << "DAY 23 (b): ";
    for (auto i = largest_clique.begin(); i != largest_clique.end(); ++i)
    {
        cout << *i;
        if (next(i) != largest_clique.end())
            cout << ",";
    }
    cout << endl;
}

int main(int argc, char **argv)
{
    ifstream file("input_d23.txt");
    string line;
    map<string, set<string>> net;

    while (getline(file, line))
    {
        string a = line.substr(0, 2);
        string b = line.substr(3);
        net[a].insert(b);
        net[b].insert(a);
    }

    cout << "DAY 23 (a): " << part_1(net) << "\n";
    part_2(net);

    return 0;
}
