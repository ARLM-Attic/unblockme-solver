#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include "UnblockMeInstance.h"

using namespace std;

int main (int argc, char** argv)
{
    if (argc > 1)
        freopen(argv[1], "r", stdin);

    vector< vector<int> > blockMap(CUnblockMeInstance::MAX_X, vector<int>(CUnblockMeInstance::MAX_Y, 0));
    for (int i = 0; i < blockMap.size(); i++)
    {
        for (int j = 0; j < blockMap[i].size(); j++)
        {
            cin >> blockMap[i][j];
        }
    }

    CUnblockMeInstance initialInstance = CUnblockMeInstance::ConstructFromMap(blockMap);
    int numBlocks = initialInstance.GetNumBlocks();
    initialInstance.PrintLayout();
    cout << endl;

    set<CUnblockMeInstance> visited;
    visited.insert(initialInstance);
    queue<CUnblockMeInstance> q;
    q.push(initialInstance);

    int currentMinSteps = 10000;
    CUnblockMeInstance winningInstance;

    while (!q.empty())
    {
        // cout << q.size() << endl;
        CUnblockMeInstance instance = q.front();
        q.pop();

        if (instance.IsWinner())
        {
            cout << "Winner: " << instance.GetSteps() << endl;
            if (instance.GetSteps() < currentMinSteps)
            {
                currentMinSteps = instance.GetSteps();
                winningInstance = instance;
            }
        }

        if (instance.GetSteps() >= currentMinSteps)
        {
            continue;
        }

        for (int i = 0; i < numBlocks; i++)
        {
            for (int j = -4; j <= 4; j++)
            {
                if (j == 0)
                    continue;

                CUnblockMeInstance instance2(instance);
                instance2.SetSteps(instance.GetSteps() + 1);
                if (instance2.MoveBlock(i, j) && visited.count(instance2) == 0)
                {
                    instance2.AddMove(i + 1, j);
                    visited.insert(instance2);
                    q.push(instance2);
                    // instance2.PrintLayout();
                    // cout << endl;
                    // Sleep(1000);
                }
            }
        }
    }

    cout << "Minimum steps: " << currentMinSteps << endl;
    cout << "Moves:" << endl;
    vector< pair<int, int> > moves = winningInstance.GetMoves();
    for (int i = 0; i < moves.size(); i++)
    {
        cout << moves[i].first << " : " << moves[i].second << endl;
    }

    return 0;
}
