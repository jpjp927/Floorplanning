#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <cmath>
#include <random>
#include <map>
#include "floorplanner.h"
using namespace std;

void Floorplanner::parseInput(fstream& input_blk_file,fstream& input_net_file)
{
    string str_blk, str_net, _;
    string blkName, netName;
    int i,l;
    // Outline
    input_blk_file >> _ >> _OutlineW  >> _OutlineH; // Outline
    input_blk_file >> _ >> _NumBlocks;
    input_blk_file >> _ >> _NumTerminals;
    
    input_net_file >> _ >> _NumNets;
    
    
    // block
    for (i=0;i<_NumBlocks;i++){
        string blkName = "";
        int w, h = 0;
        input_blk_file >> blkName >> w >> h;
        _blkArray.push_back(new Block(blkName,w,h));
        _blkName2Id[blkName] = i;
    }
    // terminal
    for (i=0;i<_NumTerminals;i++){
        string terminalName = "";
        int x, y = 0;
        input_blk_file >> terminalName >> _ >> x >> y;
        _terArray.push_back(new Terminal(terminalName,x,y));
        _terName2Id[terminalName] = _NumBlocks + i;
    }
    // net
    for (i=0;i<_NumNets;i++){
        int NetDegree = 0;
        input_net_file >> _ >> NetDegree;
        _netArray.push_back(new Net(i));
        for (l=0;l<NetDegree;l++){
            string terName = "";
            input_net_file >> terName;
            if (_blkName2Id[terName] > (_terName2Id[terName]-_NumBlocks)){
                _netArray.back()->addTerm(_blkArray[_blkName2Id[terName]]);
            }
            else{
                _netArray.back()->addTerm(_terArray[_terName2Id[terName]-_NumBlocks]);
            }
        }
        
    }



    
}
void Floorplanner::floorplan()
{
    SortBlk();
    CreateBtree();
    // _nodeQueue.push(_BstarTree->getRoot());
    // _Contour->resetContour();
    _Contour2->resetContour2();
    _BstarTree->setMaxX(0);
    _BstarTree->setMaxY(0);
    // place();
    place(nullptr, _BstarTree->getRoot(), 0);
    // cout << _Contour2->getContour().size() << endl;
    calArea();
    _BstarTree->setWL(WireLength());
    // _Anorm = _BstarTree->getArea();
    // _Wnorm = _BstarTree->getWL();
    _Anorm = 1445580;
    _Wnorm = 121042;
    calCost();
    // cout << endl;
    _BestTree->replace(_BstarTree);
    _PreTree->replace(_BstarTree);
    bestTreeUpdate();
    
    SA();
    _BstarTree->replace(_BestTree);
    plot("plot.png");
    showResult();
    
    return;
}

void Floorplanner::CreateBtree()
{
    Node* root = new Node(_blkArray[(_blkArray.size()+1)/2]);
    root->setRoot(true);
    _BstarTree->insert(nullptr, root, 0);
    _BstarTree->InitBtree(root, _blkArray,_BstarTree ,0 ,  _blkArray.size());
    
    // _BstarTree->print_bin_tree(root);
}

void Floorplanner::SortBlk()

{
    int i,l;
    for (i=0;i<_blkArray.size()/2;i++){
        for (l=0;l<_blkArray.size()/2;l++){
            if (_blkArray[i]->getWidth() > _blkArray[l]->getWidth()){
                Block* tempBlk;
                tempBlk = _blkArray[i];
                _blkArray[i] = _blkArray[l];
                _blkArray[l] = tempBlk;
                _blkName2Id[_blkArray[i]->getName()] = i;
                _blkName2Id[_blkArray[l]->getName()] = l;
            }
        }
    }
    for (i=_blkArray.size()/2;i<_blkArray.size()-_blkArray.size()/2;i++){
        for (l=_blkArray.size()/2;l<_blkArray.size()-_blkArray.size()/2;l++){
            if (_blkArray[i]->getWidth() < _blkArray[l]->getWidth()){
                Block* tempBlk;
                tempBlk = _blkArray[i];
                _blkArray[i] = _blkArray[l];
                _blkArray[l] = tempBlk;
                _blkName2Id[_blkArray[i]->getName()] = i;
                _blkName2Id[_blkArray[l]->getName()] = l;
            }
        }
    }
}


void Floorplanner::place(Node* parent, Node* node, int RL){
    if (node == nullptr)    return;

    Block* block = node->getBlock();
    bool rotate = node->getRotate();
    
    if (node->AmIRoot()){
        block->setPos(0, 0, block->getWidth(rotate), block->getHeight(rotate));
        // _Contour->upDateContour(_Contour, 0, block->getWidth(rotate), block->getHeight(rotate));
        _Contour2->updateContour2(0, block->getWidth(rotate), block->getHeight(rotate));
        node->getBlock()->_placed =2;
        
    }
    else{
        Block* parentblock = parent->getBlock();
        if (RL == 0){   //R = 0, L = 1
            // block->setPos(parentblock->getX1(), _Contour->maxInRegion(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)), parentblock->getX1() + block->getWidth(rotate),  _Contour->maxInRegion(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)) + block->getHeight(rotate));
            block->setPos(parentblock->getX1(), _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)), parentblock->getX1() + block->getWidth(rotate),  _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)) + block->getHeight(rotate));
            // _Contour->upDateContour(_Contour, block->getX1(), block->getX1() + block->getWidth(rotate), block->getHeight(rotate));
            _Contour2->updateContour2(block->getX1(), block->getX1() + block->getWidth(rotate), block->getY2());
        }

        if (RL == 1){   //R = 0, L = 1
            int y1 = 0;
            // if (_Contour->maxInRegion(parentblock->getX2(),parentblock->getX2()+block->getWidth(rotate))+block->getHeight(rotate) < parentblock->getY1()){
            //     y1 = parentblock->getY1() - block->getHeight(rotate) + 1;
            // }
            // else 
                // y1 = _Contour->maxInRegion(parentblock->getX2(),parentblock->getX2()+block->getWidth(rotate));
                y1 = _Contour2->maxInRegion2(parentblock->getX2(),parentblock->getX2()+block->getWidth(rotate));

            block->setPos(parentblock->getX2(), y1, parentblock->getX2() + block->getWidth(rotate),  y1 + block->getHeight(rotate));



            // _Contour->upDateContour(_Contour, block->getX1(), block->getX2(), block->getHeight(rotate));
            _Contour2->updateContour2( block->getX1(), block->getX2(), block->getY2());
        }
        node->getBlock()->_placed = 1;
    }
    
    // cout  << block->getX1() << ", " << block->getY1() << ", " <<block->getX2() << ", " << block->getY2() << endl;
    
    _BstarTree->setMaxX(max(block->getX2(), _BstarTree->getMaxX()));
    _BstarTree->setMaxY(max(block->getY2(), _BstarTree->getMaxY()));

    place(node, node->getChild(1), 1);
    place(node, node->getChild(0), 0);
    

}

void Floorplanner::place2(){
    _nodeQueue.push(_BstarTree->getRoot());

    while (!_nodeQueue.empty()){
        Node* node = _nodeQueue.front();
        _nodeQueue.pop();
        if(node->getChild(1))  _nodeQueue.push(node->getChild(1));
        if(node->getChild(0))  _nodeQueue.push(node->getChild(0));

        Block* block = node->getBlock();
        bool rotate = node->getRotate();
        Node* parent = node->getParent();

        if (node->AmIRoot()){
            block->setPos(0, 0, block->getWidth(rotate), block->getHeight(rotate));
            _Contour2->updateContour2(0, block->getWidth(rotate), block->getHeight(rotate));
        }

        else{
            int RL;
            if (parent->getChild(0) == node) RL = 0;
            else RL = 1;

            Block* parentblock = parent->getBlock();

            if (RL == 0){   //R = 0, L = 1
                block->setPos(parentblock->getX1(), _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)), parentblock->getX1() + block->getWidth(rotate),  _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)) + block->getHeight(rotate));
                _Contour2->updateContour2(block->getX1(), block->getX1() + block->getWidth(rotate), block->getY2());
            }

            if (RL == 1){   //R = 0, L = 1
                int y1 = 0;
                y1 = _Contour2->maxInRegion2(parentblock->getX2(),parentblock->getX2()+block->getWidth(rotate));

                block->setPos(parentblock->getX2(), y1, parentblock->getX2() + block->getWidth(rotate),  y1 + block->getHeight(rotate));

                _Contour2->updateContour2( block->getX1(), block->getX2(), block->getY2());
            }
        }

        _BstarTree->setMaxX(max(block->getX2(), _BstarTree->getMaxX()));
        _BstarTree->setMaxY(max(block->getY2(), _BstarTree->getMaxY()));



    }
}

void Floorplanner::place3(){
    _nodeQueueR.push(_BstarTree->getRoot());
    _BstarTree->setMaxX(0);
    _BstarTree->setMaxY(0);


    while (!_nodeQueueR.empty()){
        Node* node;
        if(_nodeQueueL.empty()){
            node = _nodeQueueR.front();
            _nodeQueueR.pop();
        }
        else{
            node = _nodeQueueL.front();
            _nodeQueueL.pop();
        }

        if(node->getChild(1))  _nodeQueueL.push(node->getChild(1));
        if(node->getChild(0))  _nodeQueueR.push(node->getChild(0));

        Block* block = node->getBlock();
        bool rotate = node->getRotate();
        Node* parent = node->getParent();

        if (node->AmIRoot()){
            block->setPos(0, 0, block->getWidth(rotate), block->getHeight(rotate));
            _Contour2->updateContour2(0, block->getWidth(rotate), block->getHeight(rotate));
        }

        else{
            int RL;
            if (parent->getChild(0) == node) RL = 0;
            else RL = 1;

            Block* parentblock = parent->getBlock();

            if (RL == 0){   //R = 0, L = 1
                block->setPos(parentblock->getX1(), _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)), parentblock->getX1() + block->getWidth(rotate),  _Contour2->maxInRegion2(parentblock->getX1(),parentblock->getX1() + block->getWidth(rotate)) + block->getHeight(rotate));
                _Contour2->updateContour2(block->getX1(), block->getX1() + block->getWidth(rotate), block->getY2());
            }

            if (RL == 1){   //R = 0, L = 1
                int y1 = 0;
                y1 = _Contour2->maxInRegion2(parentblock->getX2(),parentblock->getX2()+block->getWidth(rotate));

                block->setPos(parentblock->getX2(), y1, parentblock->getX2() + block->getWidth(rotate),  y1 + block->getHeight(rotate));

                _Contour2->updateContour2( block->getX1(), block->getX2(), block->getY2());
            }
        }

        _BstarTree->setMaxX(max(block->getX2(), _BstarTree->getMaxX()));
        _BstarTree->setMaxY(max(block->getY2(), _BstarTree->getMaxY()));



    }
}


double Floorplanner::WireLength()
{
    double WL = 0.;
    
    for (int i=0;i<_netArray.size();i++)
    {

        Net *net = _netArray[i];
        double x1 = 21474836;
        double y1 = 21474836;
        double x2 = 0;
        double y2 = 0;
        for (int l=0;l<net->getTermSize();l++)
        {
            Terminal *term = net->getTerm(l);
            pair<double, double> C = term->getCenter();
            x1 = min(x1, C.first);
            x2 = max(x2, C.first);
            y1 = min(y1, C.second);
            y2 = max(y2, C.second);
            // double termx, termy;
            // tie(termx, termy) = term->getCenter();
            // x1 = min(x1, termx);
            // x2 = max(x2, termx);
            // y1 = min(y1, termy);
            // y2 = max(y2, termy);
        }
        WL = WL + (x2 - x1 + y2 - y1);
    }
    return WL;
}

void Floorplanner::calArea(){
    _BstarTree->setArea( _BstarTree->getMaxX() * _BstarTree->getMaxY());
}

void Floorplanner::calCost(){
    double A = _BstarTree->getArea();
    double W = _BstarTree->getWL();
    double cost = _Alpha*(A/_Anorm) + (1 - _Alpha)*(W/_Wnorm);
    double dW = max(_BstarTree->getMaxX() - _OutlineW, 0);
    double dH = max(_BstarTree->getMaxY() - _OutlineH, 0);
    // _BstarTree->setCost(cost);
    if (((_BstarTree->getMaxX() - _OutlineW)> 0) || (_BstarTree->getMaxY() - _OutlineH) > 0)
        _BstarTree->setCost(dW/_OutlineW  + dH/_OutlineH);
    else _BstarTree->setCost(cost);
}


void Floorplanner::bestTreeUpdate(){
    if (_BestTree->getCost() > _BstarTree->getCost()){
        _BestTree->replace(_BstarTree);
        // cout << "hi " << endl;
    }
    // _PreTree->replace(_BstarTree);
    // cout << _BestTree->getCost() << ", " << _BstarTree->getCost() << endl;
}

void Floorplanner::SA(){
    double T = 1000;
    int P = 50;
    double decay = 0.95;
    srand(5435);

    while (T >= 0.01){
        for (int i=0;i<P;i++){
            int Case = rand() % 4;
            Node* node = ramdomPickNode();
            Node* node2 = ramdomPickNode();
            // _BstarTree->print_bin_tree(_BstarTree->getRoot());
            _PreTree->replace(_BstarTree);
            // _BstarTree->replace(_BestTree);
            double pre_Cost = _BstarTree->getCost();
            switch (Case){
            case 0: //rotate
                node->Rotate();
                node2->Rotate();
                // _Contour->resetContour();
                _Contour2->resetContour2();
                _BstarTree->setMaxX(0);
                _BstarTree->setMaxY(0);
                place(nullptr, _BstarTree->getRoot(), 0);
                
                // place();
                break;
            
            case 2: // delete and insert
                _BstarTree->deleteNode(node);
                _BstarTree->randomInsert(node, _BstarTree->getRoot());
                // _Contour->resetContour();
                _Contour2->resetContour2();
                // cout << "2" << endl;
                _BstarTree->setMaxX(0);
                _BstarTree->setMaxY(0);
                place(nullptr, _BstarTree->getRoot(), 0);
                // place();
                // _BstarTree->print_bin_tree(_BstarTree->getRoot());
                // cout << endl;
                // cout << "*---------------" <<endl;
                break;
            
            case 1:
                _BstarTree->randomSwap(node, node2);

                // _BstarTree->print_bin_tree(_BstarTree->getRoot());
                // _Contour->resetContour();
                _Contour2->resetContour2();
                _BstarTree->setMaxX(0);
                _BstarTree->setMaxY(0);
                place(nullptr, _BstarTree->getRoot(), 0);
                // place();
                // cout << "*---------------" <<endl;
                break;

            case 3:
                _BstarTree->randomSwapTree(node2);
                _Contour2->resetContour2();
                _BstarTree->setMaxX(0);
                _BstarTree->setMaxY(0);
                place(nullptr, _BstarTree->getRoot(), 0);
                // place();
            break;
            }

            



            calArea();
            _BstarTree->setWL(WireLength());
            calCost();
            // cout <<_BstarTree->getCost()<< endl;

            double delta = _BstarTree->getCost() - pre_Cost;
            double prob = min(1., exp(-delta / T));
            // // restore undo the operation by probability
            double r = ((double) rand() / (RAND_MAX));
            if (r > prob)
            {
                _BstarTree->replace(_PreTree);
            }

            bestTreeUpdate();
        }

        T *= decay;
    }
    
}


Node* Floorplanner::ramdomPickNode(){
    int rl = rand() % 2;
    int randomTimes = rand() % (int)log2(_blkArray.size()-1); 
    Node* now; 
    if (_BstarTree->getRoot()->getChild(rl)) now = _BstarTree->getRoot()->getChild(rl);
    else now = _BstarTree->getRoot()->getChild(!rl);
    for (int i=0;i<randomTimes;i++){
        int rl = rand() % 2;
        if (now->getChild(rl) == nullptr) break;
        now = now->getChild(rl);
    }
    return now;
}

Node* Floorplanner::ramdomPickNode2(){
    int Id = rand() % (_blkArray.size());
    if (_blkArray[Id]->getNode()->AmIRoot()){
        Id += 1;
    }
    Node* now = _blkArray[Id]->getNode(); 
    
    
    return now;
}





void Floorplanner::showResult(){
    cout << "*************************" << endl;
    cout << " Wire Length: " << _BstarTree->getWL() << endl;
    cout << " Area: " << _BstarTree->getArea() << endl;
    cout << " Cost: " << _BstarTree->getCost() << endl;
    cout << "*************************" << endl;
}


void Floorplanner::plot(string file_name)
{
    string plot_dir = "plot";
    string cmd;
    bool ret;
    cmd = "mkdir -p gnu";
    ret = system(cmd.c_str());
    cmd = "mkdir -p " + plot_dir;
    ret = system(cmd.c_str());
    string data_path = string("gnu/data") + string(".txt");
    string gnu_path = string("gnu/gnu.pt");
    ofstream data_file(data_path);
    ofstream gnu_file(gnu_path);

    // plot blocks
    int i = 0;
    for (auto block : _blkArray)
    {
        int x1, y1, x2, y2;
        tie(x1, y1) = block->get_low_xy();
        tie(x2, y2) = block->get_top_xy();
        data_file << x1 << " " << y1 << endl;
        data_file << x2 << " " << y2 << endl;
        // cout << block->getName() << ", " << x2 << endl;
        gnu_file << "set object " << i + 1 << " rect from " << x1 << "," << y1 << " to " << x2 << "," << y2 << " fillcolor lt 2 linewidth 3" << endl;
        gnu_file << "set label \"" << block->getName() << "\" at " << (x1 + x2) / 2 << "," << (y1 + y2) / 2 << " front center font \",40\"" << endl;
        i++;
    }
    // i=0;
    // for (auto con : _Contour2-> getContour()){
    //     data_file << con.front() << " " << 0 << endl;
    //     gnu_file << "set style line " << con.front() << " lc rgb 'black' pt 5 " << endl;
    //     i++;
    // }
    // plot outline
    data_file << _OutlineW << " " << _OutlineH << endl;
    gnu_file << "set arrow from 0," << _OutlineH << " to " << _OutlineW << "," << _OutlineH << " nohead lc 3 lw 5" << endl;
    gnu_file << "set arrow from " << _OutlineW << ",0 to " << _OutlineW << "," << _OutlineH << " nohead lc 3 lw 5" << endl;
    gnu_file << "set term png size 2000,2000" << endl;
    // for (auto con : _Contour2-> getContour()){
    //     gnu_file << "set arrow from " << con.front() << ",0 to " << con.front() << "," << _OutlineH*5 << " nohead lc 3 lw 6" << endl;
    //     gnu_file << "set arrow from 0," << con[2] << " to " << _OutlineW << "," << con[2] << " nohead lc 4 lw 5" << endl;
    // }
    gnu_file << "set output \'" << plot_dir << "/" << file_name << "\'" << endl;
    gnu_file << "plot \'" << data_path  << endl;
    data_file.close();
    gnu_file.close();
    string command = "gnuplot " + gnu_path;

    ret = system(command.c_str());
    cout << "Plot saved in " << plot_dir << "/" << file_name << endl;
}