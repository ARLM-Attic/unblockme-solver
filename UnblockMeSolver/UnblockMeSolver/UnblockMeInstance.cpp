#include <iostream>
#include <iomanip>
#include "UnblockMeInstance.h"

using namespace std;

CUnblockMeInstance::CUnblockMeInstance (vector< Block > layout)
    : m_layout(layout),
      m_map(vector< vector<bool> >(MAX_X, vector<bool>(MAX_Y, false))),
      m_steps(0)
{
    for (auto block = m_layout.begin(); block != m_layout.end(); block++)
    {
        for (int i = 0; i < block->size; i++)
        {
            int x = block->x + (block->orientation == Vertical ? i : 0);
            int y = block->y + (block->orientation == Horizontal ? i : 0);

            m_map[x][y] = true;
        }
    }
}

CUnblockMeInstance CUnblockMeInstance::ConstructFromMap (vector< vector<int> > map)
{
    bool stillFinding = true;
    vector< Block > blocks;
    for (int i = 1; stillFinding; i++)
    {
        stillFinding = false;
        for (int j = 0; j < map.size() && !stillFinding; j++)
        {
            for (int k = 0; k < map[j].size() && !stillFinding; k++)
            {
                if (map[j][k] != i)
                    continue;

                Block b;
                b.x = j;
                b.y = k;
                if (j < map.size() - 1 && map[j + 1][k] == i)
                    b.orientation = Vertical;
                else
                    b.orientation = Horizontal;

                b.size = 0;
                for (int l = 0; ; l++)
                {
                    int x = b.x + (b.orientation == Vertical ? l : 0);
                    int y = b.y + (b.orientation == Horizontal ? l : 0);

                    if (x >= map.size() || y >= map[x].size())
                        break;
                    if (map[x][y] != i)
                        break;

                    b.size++;
                }

                blocks.push_back(b);
                stillFinding = true;
            }
        }
    }

    return CUnblockMeInstance(blocks);
}

void CUnblockMeInstance::PrintMap ()
{
    for (int i = 0; i < m_map.size(); i++)
    {
        for (int j = 0; j < m_map[i].size(); j++)
        {
            cout << (int)m_map[i][j] << " ";
        }
        cout << endl;
    }
}

void CUnblockMeInstance::PrintLayout ()
{
    for (int i = 0; i < m_map.size(); i++)
    {
        for (int j = 0; j < m_map[i].size(); j++)
        {
            cout << setw(2) << BlockAtLocation(i, j) << " ";
        }
        cout << endl;
    }
}

int CUnblockMeInstance::BlockAtLocation (int x, int y) const
{
    for (auto block = m_layout.begin(); block != m_layout.end(); block++)
    {
        if (block->orientation == Vertical)
        {
            if (y != block->y)
                continue;

            if ((x >= block->x) && (x < (block->x +block->size)))
                return (block - m_layout.begin()) + 1;
        }
        else
        {
            if (x != block->x)
                continue;

            if ((y >= block->y) && (y < (block->y +block->size)))
                return (block - m_layout.begin()) + 1;
        }
    }

    return 0;
}

bool CUnblockMeInstance::IsWinner ()
{
    int blockId = BlockAtLocation(2, 5) - 1;
    if (blockId == -1)
        return false;

    if (m_layout[blockId].orientation != Horizontal)
        return false;

    return true;
}

vector< unsigned __int64 > CUnblockMeInstance::GetHashcode () const
{
    vector< unsigned __int64 > hashcode(3, 0);

    for (int i = 0; i < 3; i++)
    {
        unsigned __int64 hash = 0;
        for (int j = 0; j < 16; j++)
        {
            int index = i * 16 + j;
            int x = index / MAX_Y;
            int y = index % MAX_Y;

            int blockId = BlockAtLocation(x, y);
            hash = (hash << 4) | (blockId & 0xF);
        }
        hashcode[i] = hash;
    }

    return hashcode;
}

bool CUnblockMeInstance::operator < (const CUnblockMeInstance& rhs) const
{
    vector< unsigned __int64 > lhsHash = GetHashcode();
    vector< unsigned __int64 > rhsHash = rhs.GetHashcode();

    for (int i = 0; i < lhsHash.size(); i++)
    {
        if (lhsHash[i] < rhsHash[i])
            return true;
        else if (lhsHash[i] > rhsHash[i])
            return false;
    }

    return false;
}

bool CUnblockMeInstance::MoveBlock (int blockId, int offset)
{
    Block block = m_layout[blockId];
    for (int i = 1; i <= abs(offset); i++)
    {
        int x, y;
        if (offset > 0)
        {
            x = block.x + (block.orientation == Vertical ? block.size - 1 + i : 0);
            y = block.y + (block.orientation == Horizontal ? block.size - 1 + i : 0);
        }
        else
        {
            x = block.x - (block.orientation == Vertical ? i : 0);
            y = block.y - (block.orientation == Horizontal ? i : 0);
        }

        if (x >= MAX_X || x < 0 || y >= MAX_Y || y < 0)
            return false;

        if (m_map[x][y] == true)
            return false;
    }

    for (int i = 0; i < block.size; i++)
    {
        int x = block.x + (block.orientation == Vertical ? i : 0);
        int y = block.y + (block.orientation == Horizontal ? i : 0);

        m_map[x][y] = false;
    }

    m_layout[blockId].x += (m_layout[blockId].orientation == Vertical) ? offset : 0;
    m_layout[blockId].y += (m_layout[blockId].orientation == Horizontal) ? offset : 0;

    block = m_layout[blockId];

    for (int i = 0; i < block.size; i++)
    {
        int x = block.x + (block.orientation == Vertical ? i : 0);
        int y = block.y + (block.orientation == Horizontal ? i : 0);

        m_map[x][y] = true;
    }

    return true;
}
