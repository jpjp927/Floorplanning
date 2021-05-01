#ifndef PARTITIONER_H
#define PARTITIONER_H

#include <fstream>
#include <vector>
#include <map>
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
        _BestTree(new BstarTree), _PreTree(new BstarTree) {
        parseInput(input_blk_file,input_net_file);
        _Contour = new Contour(_OutlineW);
        _Contour2 = new Contour2();
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
    void bestTreeUpdate();
    void SA();
    Node* ramdomPickNode();
    Node* ramdomPickNode2();


    int getOutlineW() {return _OutlineW;}

    void setAlpha(double a) {_Alpha = a;}
    

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
    Contour*            _Contour;
    Contour2*           _Contour2;
    double              _Anorm;
    double              _Wnorm;
    queue<Node*>        _nodeQueue;
    queue<Node*>        _nodeQueueR;
    queue<Node*>        _nodeQueueL;
    // Clean up partitioner
    void clear();
};

#endif  // PARTITIONER_H