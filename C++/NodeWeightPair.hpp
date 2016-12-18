#ifndef NEURAL_NETWORKS_JAVA_VS_C_NODEWEIGHTPAIR_HPP
#define NEURAL_NETWORKS_JAVA_VS_C_NODEWEIGHTPAIR_HPP

class Node;

class NodeWeightPair{
public:
    double weight; //Weight of this connection
    Node *node; //The parent node

    NodeWeightPair(Node *node, double &weight);
};
#endif //NEURAL_NETWORKS_JAVA_VS_C_NODEWEIGHTPAIR_HPP
