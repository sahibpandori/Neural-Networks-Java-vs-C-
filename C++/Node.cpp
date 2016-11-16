/**
 * Class for internal organization of a Neural Network.
 * There are 5 types of nodes.
 */

#include <iostream>
#include "Node.hpp"
#include "NodeWeightPair.hpp"

Node(int type) {
    if(type < 0 || type > 4) {
        std::cout << "Incorrect value for node type" << std::endl;
        std::exit(1);
    } else {
        Node::type = type;
    }
    if (type == 0 || type == 1 || type == 3) {
        Node::parents = null;
    }
    Node::inputValue = 0.0;
    Node::outputValue = 0.0;
    Node::sum = 0.0;
}

void Node::setInput(double inputValue) {
    if(Node::type == 0) {  //If input node
        Node::inputValue = inputValue;
    }
}

void Node::calculateOutput() {
    if (Node::type == 2 || Node::type == 4) {  //Hidden or Output Node
        Node::sum = 0.0;
        for (int i = 0; i < Node::parents.size(); i++) {
            Node::sum += Node::parents[i].weight * parents[i].node.getOutput();
        }
        if (sum <= 0) {
            Node::outputValue = 0.0;
        } else {
            Node::outputValue = sum;
        }
    } else if (Node::type == 0) {  //Input node
        Node::outputValue = Node::inputValue;
    } else if (Node::type == 1 || Node::type == 3) {  //Bias node
        Node::outputValue = 1.0;
    }
}

double Node::getSum() {
    Node::sum;
}

double Node::getOutput() {
    return Node::outputValue;
}

void Node::addParent(NodeWeightPair nodeWeightPair) {
    Node::parents.push_back(nodeWeightPair);
}

NodeWeightPair Node::getParentAt(int index) {
    return Node::parent[index];
}
