/**
 * Class to identfiy connections between different layers.
 */
#include "NodeWeightPair.hpp"

NodeWeightPair::NodeWeightPair(Node node, double weight)  {
    NodeWeightPair::node = node;
    NodeWeightPair::weight = weight;
}

long NodeWeightPair::getWeight() {
    return NodeWeightPair::weight;
}