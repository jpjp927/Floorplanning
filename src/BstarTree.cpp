#include "BstarTree.h"
#include "module.h"
#include "iostream"

using namespace std;

bool BstarTree::insert(Node *parent, Node *node, int RL)
{
    // if ((parent!=nullptr)&&(parent->getChild(RL) != nullptr))    return false;

    if(parent==nullptr){
        setRoot(node);
    }
    else{
        parent->setChild(node, RL);
        node->setParent(parent);
    }
    return true;
}

void BstarTree::InitBtree(Node* node, vector<Block*> blkArray, BstarTree* _BstarTree, int start, int end)
{
    if (start <= end)
    {
        // _BstarTree->insert(nullptr, root, 0);
        int now = (start + end + 1) / 2;
        if (now > blkArray.size()-1) return;
        
        int left = (start + now +1) / 2;
        int right = (now + end +1) / 2;

        // if (right >= blkArray.size()) return;
        // cout << start << ", " << end << ", " << left << ", " << right<< endl;
        
        if (blkArray[left]->getNode() == nullptr) {
            Node* nodeL = new Node(blkArray[left]);
            insert(node, nodeL, 1);
            InitBtree(nodeL, blkArray,_BstarTree, start, now );
            // cout << "11111111" << endl;
        }
        if (right >= blkArray.size()) return;
        if (blkArray[right]->getNode() == nullptr){
            Node* nodeR = new Node(blkArray[right]);
            insert(node, nodeR, 0);
            InitBtree(nodeR, blkArray,_BstarTree, now , end);
            // cout << "11111111" << endl;
        }
        if ((blkArray[0]->getNode() == nullptr) && (blkArray[left]->getNode() != nullptr) && blkArray[right]->getNode() != nullptr){
            Node* node0 = new Node(blkArray[0]);
            insert(node, node0, 0);
        }
    }
}


void BstarTree::deleteTree(Node* oldNode){
    if(oldNode == nullptr) return;
    if(oldNode->AmIRoot()) root = nullptr;
    deleteTree(oldNode->getChild(0));
    deleteTree(oldNode->getChild(1));
    oldNode->getBlock()->setNode(nullptr);
    delete oldNode;
}


void BstarTree::replace(BstarTree* otherTree){
    
    deleteTree(root);
    // otherTree->print_bin_tree(otherTree->getRoot());
    clone(otherTree->getRoot(), nullptr, 0);
    setCost(otherTree->getCost());
}

void BstarTree::clone(Node *oldNode, Node* newParent, int RL){
    if(oldNode == nullptr) return;
    // cout << oldNode->getBlock()->getName() << endl;
    if (oldNode->getBlock() == nullptr) cout << "error !" <<endl;
    // cout << oldNode << endl;
    Node* newNode = new Node(oldNode);
    insert(newParent, newNode, RL);
    clone(oldNode->getChild(1), newNode, 1);
    clone(oldNode->getChild(0), newNode, 0);
}


void BstarTree::print_bin_tree(Node* node)
{
    if (node != nullptr)
    {
        cout << node->getBlock()->getName() << " ";
        if (node->getChild(1) != nullptr)print_bin_tree(node->getChild(1));
        if (node->getChild(0) != nullptr)print_bin_tree(node->getChild(0));
            
    }
}


void BstarTree::randomInsert(Node* node, Node* pos){
    int rl = rand() % 2;
    if (pos->getChild(rl) == nullptr){
        insert(pos,node,rl);
    }
    else randomInsert(node, pos->getChild(rl));
    
}



void BstarTree::deleteNode(Node* node){
    Node* _node;
    // if (node->AmIRoot()) {
    //     int rl = rand() % 2;
    //     if(node->getChild(rl)) _node = node->getChild(rl);
    //     else _node = node->getChild(!rl);
    //     if ((node->getChild(rl)== nullptr) && (node->getChild(!rl)== nullptr)) cout << "error " <<endl;
    // }
    // else 
    _node = node;


    Node* nodeR = _node->getChild(0);
    Node* nodeL = _node->getChild(1);
    Node* parentNode = _node->getParent();
    int RLofParent;
    if (parentNode->getChild(0) == _node) RLofParent = 0;
    else RLofParent = 1;

    if ((nodeR == nullptr) && (nodeL == nullptr)) {
        parentNode->setChild(nullptr, RLofParent);
        _node->setParent(nullptr);
        _node->setChild(nullptr, 0);
        _node->setChild(nullptr, 1);
    }
    else if (nodeR == nullptr) {
        parentNode->setChild(nodeL, RLofParent);
        nodeL->setParent(parentNode);
        _node->setParent(nullptr);
        _node->setChild(nullptr, 0);
        _node->setChild(nullptr, 1);
    }
    else if (nodeL == nullptr) {
        parentNode->setChild(nodeR, RLofParent);
        nodeR->setParent(parentNode);
        _node->setParent(nullptr);
        _node->setChild(nullptr, 0);
        _node->setChild(nullptr, 1);
    }
    else{
        int rl = rand() % 2;
        swap(_node, _node->getChild(rl),rl);
        deleteNode(_node);
    }
}

void BstarTree::swap(Node* parent,Node* child, int RL){
    Node* pparent = parent->getParent();
    if (pparent == nullptr) return;
    int RLofParent;
    if (pparent->getChild(0) == parent) RLofParent = 0;
    else RLofParent = 1;
    // cout << RLofParent << endl;
    pparent->setChild(child, RLofParent);
    child->setParent(pparent);

    if(parent->getChild(!RL)) parent->getChild(!RL)->setParent(child);

    if(child->getChild(!RL) != nullptr) child->getChild(!RL)->setParent(parent);
    Node* temp = parent->getChild(!RL);
    parent->setChild(child->getChild(!RL), !RL);

    child->setChild(temp, !RL);

    parent->setChild(child->getChild(RL), RL);
    if(child->getChild(RL) != nullptr)child->getChild(RL)->setParent(parent);

    child->setChild(parent, RL);
    parent->setParent(child);
}


void BstarTree::randomSwap(Node* node1,Node* node2){
    if (node1 == node2) return;
    if ((node1 == nullptr) || (node2 == nullptr)) return;
    if (node1 = node2->getParent()) {
        int RLofNode2;
        if (node1->getChild(0) == node2) RLofNode2 = 0;
        else RLofNode2 = 1;
        swap(node1, node2, RLofNode2);
        return;
    }
    else if (node2 = node1->getParent()){
        int RLofNode1;
        if (node2->getChild(0) == node1) RLofNode1 = 0;
        else RLofNode1 = 1;
        swap(node2, node1, RLofNode1);
        return;
    }


    Node* tempNode = new Node(node1, 0);

    int RLofNode2;
    int RLofNode1;
    if (node2->getParent()->getChild(0) == node2) RLofNode2 = 0;
    else RLofNode2 = 1;
    if (node1->getParent()->getChild(0) == node1) RLofNode1 = 0;
    else RLofNode1 = 1;
    
    node1->setParent(node2->getParent());
    node1->setChild(node2->getChild(1), 1);
    node1->setChild(node2->getChild(0), 0);
    node2->getParent()->setChild(node1, RLofNode2);
    
    if(node2->getChild(1) != nullptr) node2->getChild(1)->setParent(node1);
    if(node2->getChild(0) != nullptr) node2->getChild(0)->setParent(node1);
    
    node2->setParent(tempNode->getParent());
    node2->setChild(tempNode->getChild(1), 1);
    node2->setChild(tempNode->getChild(0), 0);
    tempNode->getParent()->setChild(node2, RLofNode1);
    if(tempNode->getChild(1) != nullptr) tempNode->getChild(1)->setParent(node2);
    if(tempNode->getChild(0) != nullptr) tempNode->getChild(0)->setParent(node2);

    delete tempNode;
}

void BstarTree::randomSwapTree (Node* node){
    Node* node0 = node->getChild(0);
    Node* node1 = node->getChild(1);

    node->setChild(node1, 0);
    node->setChild(node0, 1);
}




void Contour::upDateContour(Contour* contour, int startX, int endX, int hight){
    if (startX == 0) {
        int hightest = maxInRegion(0,endX);
        for (int i = startX;i<endX;i++){
            contour->setYpair(i, hightest + hight, hightest + hight);
        }
        contour->setYpair(endX, hightest+hight, getContour(endX).second );
        // cout << hightest << ", " << hight << ", " << endX <<endl;
    }

    else{
        int hightest = 0;
        for (int i = startX+1;i<endX;i++){
            if (max(getContour(i).second,getContour(i).first) > hightest)
                hightest = max(getContour(i).second,getContour(i).first);
        }
        if (getContour(startX).second > hightest) hightest = getContour(startX).second;
        if (getContour(endX).first > hightest) hightest = getContour(endX).first;

        for (int i = startX+1;i<endX;i++){
            contour->setYpair(i, hightest + hight, hightest + hight);
        }
        contour->setYpair(startX, getContour(startX).first, hightest + hight );
        contour->setYpair(endX, hightest + hight , getContour(endX).second);
    }
}

void Contour::resetContour(){
    for (int i=0;i<_yContour.size();i++){
        _yContour[i] = pair<int,int> (0,0);
    }
}



int Contour::maxInRegion( int startX, int endX){
    int hightest = 0;

    for (int i=startX+1;i<endX; i++){
        if (max(getContour(i).second,getContour(i).first) > hightest)
            hightest = max(getContour(i).second,getContour(i).first);
    }
    if (getContour(startX).second > hightest) hightest = getContour(startX).second;
    if (getContour(endX).first > hightest) hightest = getContour(endX).first;

    return hightest;
}


void Contour2::updateContour2(int start, int end, int y2){
    bool flag = false;
    bool flag2 = false;
    int is = 0;
    int ie = 0;

    for (int i=0;i<_yContour2.size();i++){
        if ((_yContour2[i].front() == start) && (flag == false)){
            _yContour2[i] = {start, _yContour2[i][1], y2};
            flag = true;
            is = i;
            break;
        }
        if ((_yContour2[i].front() > start) && (flag == false)){
            insertBefore(i,{ start, _yContour2[i][1], y2 });
            flag = true;
            is = i;
            break;
        }
    }

    for (int i=0;i<_yContour2.size();i++){
        if ((_yContour2[i].front() == end) && (flag2 == false)){
            _yContour2[i] = {end, y2, _yContour2[i][2]};
            flag2 = true;
            ie = i;
            break;
        }

        if ((_yContour2[i].front() > end) && (flag2 == false)){
            insertBefore(i,{ end, y2, _yContour2[i][1] });
            flag2 = true;
            ie = i;
            break;
        }
    }
    if((is+1)!=ie) _yContour2.erase(_yContour2.begin()+is+1,_yContour2.begin()+ie);
    if (getPreY1(is) == getPreY2(is)) _yContour2.erase(_yContour2.begin()+is);
    // if (getPreY1(ie) == getPreY2(ie)) _yContour2.erase(_yContour2.begin()+ie);

}

int Contour2::maxInRegion2(int startX, int endX){
    int maxi = 0;
    for (int i=0;i<_yContour2.size()-1;i++){
        if ((_yContour2[i].front() > startX) && (_yContour2[i].front() < endX)){
            if (max(getPreY1(i),getPreY2(i)) > maxi){
                maxi = max(getPreY1(i),getPreY2(i));
            }
        }
        if (_yContour2[i].front() == startX) maxi = max(maxi, getPreY2(i));
        if (_yContour2[i].front() == endX) maxi = max(maxi, getPreY1(i));

        if ((_yContour2[i].front() < startX) && (_yContour2[i].front() < endX) && (_yContour2[i+1].front() > startX) && (_yContour2[i+1].front() > endX)){
            maxi = getPreY2(i);
        }

        if (_yContour2[i].front() > endX) return maxi; //提早結束
    }
    return maxi;
}

void Contour2::resetContour2(){
    _yContour2.clear();
    _yContour2.push_back({0,0,0}); 
    _yContour2.push_back({2147483647,0,0}); 
}



