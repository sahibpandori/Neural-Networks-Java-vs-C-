/**
 * Class for internal organization of a Neural Network.
 * There are 5 types of nodes.
 */

#include <iostream>
#include "Node.hpp"

Node::Node(int type) {
    if(type < 0 || type > 4) {
        std::cout << "Incorrect value for node type" << std::endl;
        std::exit(1);
    } else {
        this->type = type;
    }
   if (type == 0 || type == 1 || type == 3) {
       this->parents.clear();
   }
    this->inputValue = 0.0;
    this->outputValue = 0.0;
    this->sum = 0.0;
}

void Node::setInput(double inputValue) {
    if(this->type == 0) {  //If input node
        this->inputValue = inputValue;
    }
}

void Node::calculateOutput() {
    if (this->type == 2 || this->type == 4) {  // Hidden or Output Node
        this->sum = 0.0;
        for (int i = 0; i < this->parents.size(); i++) {
            this->sum += this->parents[i].getWeight() * parents[i].getNode()->getOutput();
        }
        if (sum <= 0) {
            this->outputValue = 0.0;
        } else {
            this->outputValue = sum;
        }
    } else if (this->type == 0) {  // Input node
        this->outputValue = this->inputValue;
    } else if (this->type == 1 || this->type == 3) {  //Bias node
        this->outputValue = 1.0;
    }
}

double Node::getSum() {
    return this->sum;
}

double Node::getOutput() {
    return this->outputValue;
}

void Node::addParent(NodeWeightPair &nodeWeightPair) {
    this->parents.push_back(nodeWeightPair);
}

NodeWeightPair& Node::getParentAt(int index) {
    return this->parents[index];
}
