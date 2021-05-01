#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
using namespace std;

class Node;


class Terminal
{
public:
    // constructor and destructor
    Terminal(string& name, int x, int y) :
        _name(name), _x1(x), _y1(y), _x2(x), _y2(y) { }
    ~Terminal()  { }

    // basic access methods
    const string getName()  { return _name; }
    const int getX1()    { return _x1; }
    const int getX2()    { return _x2; }
    const int getY1()    { return _y1; }
    const int getY2()    { return _y2; }

    // set functions
    void setName(string& name) { _name = name; }
    void setPos(int x1, int y1, int x2, int y2) {
        _x1 = x1;   _y1 = y1;
        _x2 = x2;   _y2 = y2;
    }
    
    pair<double, double> getCenter() const { return pair<double, double>((double(_x1+_x2))/2, (double(_y1+_y2))/2); }

protected:
    string      _name;      // module name
    int      _x1;        // min x coordinate of the terminal
    int      _y1;        // min y coordinate of the terminal
    int      _x2;        // max x coordinate of the terminal
    int      _y2;        // max y coordinate of the terminal
};





class Block : public Terminal
{
public:
    // constructor and destructor
    Block(string& name, int w, int h) :
        _name(name), Terminal(name, 0, 0), _w(w), _h(h), _node(nullptr),_placed(0) { }
    ~Block() { }

    // basic access methods
    const int getWidth(bool rotate = false)  { return rotate? _h: _w; }
    const int getHeight(bool rotate = false) { return rotate? _w: _h; }
    const int getArea()  { return _h * _w; }
    // static int getMaxX() { return _maxX; }
    // static int getMaxY() { return _maxY; }
    string getName() const  { return _name; }
    Node* getNode() const{return _node;}
    

    pair<int, int> get_low_xy() const { return pair<int, int>(_x1, _y1); }
    pair<int, int> get_top_xy() const { return pair<int, int>(_x2, _y2); }


    // set functions
    void setWidth(int w)         { _w = w; }
    void setHeight(int h)        { _h = h; }
    // static void setMaxX(int x)   { _maxX = x; }
    // static void setMaxY(int y)   { _maxY = y; }
    void setName(const string name) { _name = name; }
    void setNode(Node* node){_node = node;}
    int _placed;

private:
    int          _w;         // width of the block
    int          _h;         // height of the block
    // static int   _maxX;      // maximum x coordinate for all blocks
    // static int   _maxY;      // maximum y coordinate for all blocks

    string          _name;      // name of the blk
    Node*           _node;
    
};


class Net
{
public:
    // constructor and destructor
    Net(int id):_id(id)   { }
    ~Net()  { }

    // basic access methods
    const vector<Terminal*> getTermList()   { return _termList; }
    int get_id() const{return _id;}

    // modify methods
    void addTerm(Terminal* term) { _termList.push_back(term); }
    Terminal* getTerm(int i) {return _termList[i];};
    int getTermSize(){return _termList.size();}
    // other member functions
    double calcHPWL();

private:
    vector<Terminal*>   _termList;  // list of terminals the net is connected to
    int                 _id;
};



class Node 
{
public:
    Node(Block* block) :   _left(nullptr), _right(nullptr), _parent(nullptr),  
                        _block(block), _IamRoot(false), _rotate(false) {block->setNode(this);}
    Node(Node* node) :   _left(node->getChild(1)), _right(node->getChild(0)), _parent(node->getParent()),  
                        _block(node->getBlock()), _rotate(false), _IamRoot(node->AmIRoot()) {_block->setNode(this);}
    Node(Node* node, int choose) :   _left(node->getChild(1)), _right(node->getChild(0)), _parent(node->getParent()),  
                        _block(node->getBlock()), _rotate(false), _IamRoot(node->AmIRoot()) {}
    ~Node()  { }

    // getter
    // int get_id() const{return _id;}
    Block*  getBlock() const{return _block;}
    Node*   getParent() const{return _parent;}
    Node*   getChild(int RL) const{
        if (RL == 0) return _right;
        else return _left;
    }
    bool getRotate() {return _rotate;}
    bool    AmIRoot() const {return _IamRoot;}

    void setParent(Node *node){_parent = node;}
    void setRoot(bool yn) {_IamRoot = yn;}
    void setChild(Node *node,int RL){  // R=0 , L=1
        if (RL == 0) _right = node;
        else _left = node;
    }
    void Rotate() {
        // _block->setPos(_block->getX1(), _block->getY1(), _block->getX1() + _block->getHeight(), _block->getY1() + _block->getWidth());
        _rotate = !_rotate;

    }


private:
    // int                 _id;
    bool                _rotate;
    Node*               _left;
    Node*               _right;
    Node*               _parent;
    Block*              _block;
    bool                _IamRoot;
};




#endif  // MODULE_H
