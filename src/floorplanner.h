#ifndef PARTITIONER_H
#define PARTITIONER_H

#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include "module.h"
#include "BstarTree.h"
#include <math.h> 
#include <queue>
using namespace std;


class Floorplanner
{
public:
    // constructor and destructor
    Floorplanner(fstream& input_blk_file,fstream& input_net_file) :
        _OutlineW(0), _OutlineH(0), _NumBlocks(0), _NumTerminals(0),
        _NumNets(0), _BstarTree(new BstarTree), _Anorm(1), _Wnorm(1), 
        _BestTree(new BstarTree), _PreTree(new BstarTree),
        start_time(clock()), _legalbestTree(new BstarTree) {
        parseInput(input_blk_file,input_net_file);
        _Contour = new Contour(_OutlineW);
        _Contour2 = new Contour2();
        _Contour3 = new Contour3();
        // _partSize[0] = 0;
        // _partSize[1] = 0;
    }
    ~Floorplanner() {
        clear();
    }   

    // modify method
    void parseInput(fstream& input_blk_file,fstream& input_net_file);
    void SortBlk();
    void CreateBtree(Node* node, vector<Block*> blkArray, int start, int end);
    void floorplan();
    void CreateBtree();
    void place(Node* parent, Node* node, int RL);
    void place2();
    void place3();
    double WireLength();
    void plot(string file_name);
    void showResult();
    void calArea(); 
    void calCost(); 
    bool bestTreeUpdate();
    void SA();
    void deterInit();
    Node* ramdomPickNode();
    Node* ramdomPickNode2();
    Node* rightestNode(Node* node);
    Node* leftestNode(Node* node);
    void write();

    int getOutlineW() {return _OutlineW;}

    void setAlpha(double a) {_Alpha = a;}
    double get_time() const {return (double)(clock() - start_time) / CLOCKS_PER_SEC;}

    // void setNorm(double A, double W) {_Anorm = A;   _Wnorm = W;}

    


private:
    int                 _OutlineW;        // outline width
    int                 _OutlineH;        // outline height
    int                 _NumBlocks;
    int                 _NumTerminals;
    int                 _NumNets;
    double              _Alpha;
    
    vector<Net*>        _netArray;      // net array of the circuit
    vector<Block*>      _blkArray;     // blk array of the circuit
    vector<Terminal*>   _terArray;     // blk array of the circuit
    map<string, int>    _terName2Id;
    map<string, int>    _blkName2Id;
    BstarTree*          _BstarTree;
    BstarTree*          _BestTree;
    BstarTree*          _PreTree;
    BstarTree*          _legalbestTree;
    Contour*            _Contour;
    Contour2*           _Contour2;
    Contour3*           _Contour3;
    double              _Anorm;
    double              _Wnorm;
    queue<Node*>        _nodeQueue;
    queue<Node*>        _nodeQueueR;
    queue<Node*>        _nodeQueueL;
    clock_t start_time;
    double time;
    // Clean up partitioner
    void clear();
};

#endif  // PARTITIONER_H