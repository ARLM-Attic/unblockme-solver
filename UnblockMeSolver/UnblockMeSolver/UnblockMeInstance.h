#include <vector>

using namespace std;

enum BlockOrientation
{
    Horizontal,
    Vertical
};

typedef struct
{
    int x;
    int y;
    int size;
    BlockOrientation orientation;
} Block;

class CUnblockMeInstance
{
public:

    CUnblockMeInstance (vector< Block > layout);

    CUnblockMeInstance () {}

    bool MoveBlock (int blockId, int offset);

    void PrintMap ();

    void PrintLayout ();

    bool IsWinner ();

    void AddMove (int blockId, int offset) { m_moves.push_back(make_pair(blockId, offset)); }

    vector< pair<int, int> > GetMoves() { return m_moves; }

    int GetSteps () { return m_steps; }
    void SetSteps (int steps) { m_steps = steps; }

    vector< unsigned __int64 > GetHashcode () const;

    bool operator < (const CUnblockMeInstance& rhs) const;

    int GetNumBlocks () { return m_layout.size(); }

    static CUnblockMeInstance ConstructFromMap (vector< vector<int> > map);

    static const int MAX_X = 6;
    static const int MAX_Y = 6;

private:

    int BlockAtLocation (int x, int y) const;

    vector< Block > m_layout;

    vector< vector<bool> > m_map;

    vector< pair<int, int> > m_moves;

    int m_steps;
};
