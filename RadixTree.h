#include <vector>
#include <map>
#include <utility>
#include <string>
#include <iostream>

#ifndef DATA_STRUCTURES_RADIXTREE_H
#define DATA_STRUCTURES_RADIXTREE_H


template <typename ValueType>
class RadixTree {
public:
    RadixTree();

    ~RadixTree(){//destructor
        iterateAndDelete(root);
    };

    void insert(std::string key, const ValueType& value);

    void remove(std::string key);
    int update(std::string key, const ValueType& value);

    ValueType* search(std::string key) const;


private:

    int getIndexOfMisMatch(std::string key, std::string edge); //gets index of the mismatch between string and edge

    class Node;
    class Map;

    void iterateAndDelete(Node* root); //deletes all nodes for destructor

    Node* removeHelper(std::string key, Node* curr); //recursive helper function for delete

    struct Edge{ //Edge structure that holds labels
        Edge(std::string label);

        bool operator<(const Edge& e1);

        Edge(std::string label, Node *next);

        Node* next;
        std::string label;
    };

    struct Node{ //Node structure that holds keys
        Node(bool isLeaf);

        void addEdge(std::string label, Node* next);

        Node& operator=(const Node& n);

        bool isLeaf;

        Map edges;
        ValueType *val;

    };

    struct Map{ //map to store first character difference to walk through tree and get next edge
        Map(){
            for (int i = 0; i < 128; ++i) {
                map[i] = nullptr;
            }
        }
        ~Map(){
            for (int i = 0; i < 128; ++i) {
                delete map[i];
            }
        }
        void insert(char c, Edge* e);

        void remove(char c);

        int count();

        Edge * get(char c);

    private:
        Edge* map [128];
    };

    Node* root; //for use by Map, Node, and Edge structures
};

template<typename ValueType>
void RadixTree<ValueType>::Map::insert(char c, RadixTree::Edge *e) { //inserts mapping character to edge
    map[c] = e;
}
template<typename ValueType>
void RadixTree<ValueType>::Map::remove(char c) {//removes mapping character for edge
    map[c] = nullptr;
}

template<typename ValueType>
typename RadixTree<ValueType>::Edge *RadixTree<ValueType>::Map::get(char c) {
    return map[c]; //gets mapping character for an edge
}

template<typename ValueType>
int RadixTree<ValueType>::Map::count() { //counts how many edges a node has
    int count = 0;
    for (int i = 0; i < 128; ++i) {
        if(map[i] != nullptr){
            count++;
        }
    }
    return count;

}


template<typename ValueType>
RadixTree<ValueType>::Edge::Edge(std::string label) : label(label) {//Edge constructor
    next = new Node(true);
}

template<typename ValueType>
RadixTree<ValueType>::Edge::Edge(std::string label, RadixTree::Node *next)  : next(next), label(label) {} //Edge Constructor

template<typename ValueType>
bool RadixTree<ValueType>::Edge::operator<(const RadixTree::Edge &e1) { //Edge operator for comparison
    return this->label < e1.label;
}


template<typename ValueType>
RadixTree<ValueType>::Node::Node(bool isLeaf)  : isLeaf(isLeaf) { //Node constructor
    val = new ValueType;
}


template<typename ValueType>
void RadixTree<ValueType>::Node::addEdge(std::string label, RadixTree::Node *next) {//Adds edge to a node
    edges.insert(label[0], new Edge(label, next));
}

template<typename ValueType>
typename RadixTree<ValueType>::Node &RadixTree<ValueType>::Node::operator=(const Node& n) { //equality operator overload
    if(this == &n){
        return *this;
    }
    this->isLeaf = n.isLeaf;
    this->val = n.val;
    this->edges = n.edges;
    return *this;
}

template<typename ValueType>
RadixTree<ValueType>::RadixTree() { //Radix Tree Constructor
    root = new Node(false);
}


template<typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType &value) { //inserting a key, value pair into radix tree
    Node* curr = root;
    int found = 0;

    while(found < key.length()){
        Edge *e = curr->edges.get(key[found]);
        std::string s = key.substr(found);
        //no associated edge with first character case
        if(e == nullptr){
            Edge *e2 = new Edge(s);
            (*e2->next->val) = value;
            curr->edges.insert(key[found], e2);
            break;
        }

        int ind = getIndexOfMisMatch(s, e->label); //gets first character of mismatch
        if(ind == -1){//no mismatch
            //case, the edge and rest of string same length
            if(s.length() == e->label.length()){
                e->next->isLeaf = true;
                break;
            }
            else if(s.length() < e->label.length()){
                std::string suf = e->label.substr(s.length());
                e->label = s;
                Node *newNode = new Node(true);
                Node *newnewNode = e->next;
                newnewNode->val = e->next->val;
                (*newNode->val) = value;
                e->next = newNode;
                newNode->addEdge(suf, newnewNode);
                break;
            }
            else{//string length index left after match with no mismatch
                ind = e->label.length();
            }
        }
        else{
            //string and edge mismatch, so split and create new nodes
            std::string suf = e->label.substr(ind);
            e->label = e->label.substr(0, ind);
            Node* prev = e->next;
            ValueType *temp = e->next->val;
            e->next = new Node(false);
            prev->val = temp;
            e->next->addEdge(suf, prev);
        }
        //gets next tree node
        curr = e->next;
        found += ind;
    }
}

template<typename ValueType>
ValueType *RadixTree<ValueType>::search(std::string key) const {
    std::string s = key;
    Node* curr = root;
    int found = 0;
    while(found < key.length()){
        Edge* e = curr->edges.get(key[found]); //gets next edges and iterate throught the tree
        if(e == nullptr){
            return nullptr;
        }

        std::string sub = key.substr(found);
        if(sub.rfind(e->label, 0) != 0){//if not a prefix
            return nullptr;
        }
        found += e->label.length();
        curr = e->next;
    }
    if(curr->isLeaf){
        //checks if the node is a leaf, which means it holds a value
        return curr->val;
    }
    return nullptr;
}

template<typename ValueType>
int RadixTree<ValueType>::getIndexOfMisMatch(std::string key, std::string edge) {//gets index of first string mismatch with edge
    int len = key.length();
    if(len > edge.length()){
        len = edge.length();//get max length of either strings
    }
    for (int i = 1; i < len; ++i) {
        if(key[i] != edge[i]){//index of first mismatch
            return i;
        }
    }
    return -1;
}

template<typename ValueType>
void RadixTree<ValueType>::iterateAndDelete(RadixTree::Node *root) {//deletes tree for destructor
    if(root == nullptr){
        return;
    }
    for (int i = 0; i < 128; ++i) {
        if(root->edges.get(i)){
            iterateAndDelete(root->edges.get(i)->next);//recursively call to delete nodes
        }
    }
    delete root->val;//delete key
    delete root;//delete node
}

template<typename ValueType>
void RadixTree<ValueType>::remove(std::string key) { //remove node interface
    removeHelper(key, root);//call to helper function that does the delete recursviely
}

template<typename ValueType>
typename RadixTree<ValueType>::Node *RadixTree<ValueType>::removeHelper(std::string key, RadixTree::Node* curr) {//recursive function to remove a node
    if(key.length() == 0){
        if(curr->edges.count() == 0 && curr != root){//if no other edges then return
            return nullptr;
        }
        curr->isLeaf = true;
        return curr;
    }
    Edge* e = curr->edges.get(key[0]);//transverses edges

    if(e == nullptr || key.rfind(e->label, 0) != 0){//has no edge for key or is not a prefix
        return curr;
    }

    Node *removed = removeHelper(key.substr(e->label.length()), e->next); //recursively call to delete

    if(removed == nullptr){//if removed is nullptr
        curr->edges.remove(key[0]);
        if(curr->edges.count() == 0 && !curr->isLeaf && curr != root){//if no edges and is not a leaf, we can remove
            return nullptr;
        }
    }
    else if(removed->edges.count() == 1 && !removed->isLeaf){// has an edge and is not a leaf, we have to update edges
        curr->edges.remove(key[0]);
        for (int i = 0; i < 128; ++i) {
            Edge *r = removed->edges.get(i);
            if(r != nullptr){
                curr->addEdge(e->label + r->label, r->next);
            }
        }

    }
    return curr;
}

template<typename ValueType>
int RadixTree<ValueType>::update(std::string key, const ValueType &value) {//updates the key of a node, returns 0 on success and -1 on failure
    std::string s = key;
    Node* curr = root;
    int found = 0;
    while(found < key.length()){//iterates through the tree
        Edge* e = curr->edges.get(key[found]);
        if(e == nullptr){
            return -1;
        }

        std::string sub = key.substr(found);
        if(sub.rfind(e->label, 0) != 0){//if not a prefix, then it is not in the tree
            return -1;
        }
        found += e->label.length();
        curr = e->next;
    }
    if(curr->isLeaf){ //if the value was found
        *curr->val = value; //update value
        return 0;
    }
    return -1;

}


#endif //DATA_STRUCTURES_RADIXTREE_H
