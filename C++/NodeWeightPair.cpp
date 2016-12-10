/**
 * Class to identfiy connections between different layers.
 */
#include "NodeWeightPair.hpp"

NodeWeightPair::NodeWeightPair(Node *node, double &weight)  {
    NodeWeightPair::node = node;
    NodeWeightPair::weight = weight;
}

double NodeWeightPair::getWeight() {
    return NodeWeightPair::weight;
}

void NodeWeightPair::addWeight(double weight) {
    NodeWeightPair::weight += weight;
}

Node* NodeWeightPair::getNode() {
    return NodeWeightPair::node;
}
