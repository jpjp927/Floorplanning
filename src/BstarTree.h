#include "module.h"
// #include "floorplanner.h"
#include <utility>

using namespace std;

class BstarTree
{
public:
    BstarTree() : root(nullptr), cost(2147483647), _wirelength(0.),_maxX(0) ,_maxY(0),_Area(1) {}
    ~BstarTree()  { }

    Node* getRoot() const{return root;}
    double getCost() const{return cost;}
    double getWL() {return _wirelength;}
    int getMaxX() { return _maxX; }
    int getMaxY() { return _maxY; }
    int getArea() { return _Area; }

    void setRoot(Node *node){root = node;}
    void setCost(double incost){cost = incost;}
    void setMaxX(int x)   { _maxX = x; }
    void setMaxY(int y)   { _maxY = y; }
    void setWL(double WL) {_wirelength = WL;}
    void setArea(int a) {_Area = a;}

    bool insert(Node *parent, Node *node, int RL);
    void InitBtree(Node* node, vector<Block*> blkArray, BstarTree* _BstarTree, int start, int end);
    void print_bin_tree(Node* node);
    void clone(Node *oldNode, Node* newParent, int RL);
    void replace(BstarTree* otherTree);
    void deleteTree(Node* oldNode);
    void deleteNode(Node* node);
    void swap(Node* parent,Node* child, int RL);
    void randomInsert(Node* node, Node* pos);

private:
    Node *root;
    double cost;
    double _wirelength;
    int    _Area;
    int   _maxX;      // maximum x coordinate for all blocks
    int   _maxY;      // maximum y coordinate for all blocks
};



class Contour
{
public:
    Contour(int W): _W(W) {
        for (int i=0; i<2*W; i++)
            _yContour.push_back( make_pair(0,0) );
    }
    ~Contour()  { }

    void setYpair(int x, int y1, int y2) {_yContour[x] = pair<int,int> (y1,y2);}

    pair<int, int> getContour(int x) {return _yContour[x];}

    void upDateContour(Contour* contour, int startX, int endX, int hight);
    int maxInRegion(int startX, int endX);
    
    void resetContour();
    
private:
    vector<pair<int, int>> _yContour;
    int _W;

};