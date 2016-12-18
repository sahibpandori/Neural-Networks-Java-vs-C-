/**
 * Class to identfiy connections between different layers.
 */
#include "NodeWeightPair.hpp"

NodeWeightPair::NodeWeightPair(Node *node, double &weight)  {
    this->node = node;
    this->weight = weight;
}
