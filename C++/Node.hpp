#ifndef NEURAL_NETWORKS_JAVA_VS_C_NODE_HPP
#define NEURAL_NETWORKS_JAVA_VS_C_NODE_HPP

#include <vector>
#include "NodeWeightPair.hpp"

class Node {
private:
    int type; //0 = input,1 = biasToHidden,2 = hidden,3 = biasToOutput,4 = Output
    double inputValue;
    double outputValue;
    double sum; // sum of wi*xi

public:
    std::vector<NodeWeightPair> parents; //Vector that will contain the parents (including the bias node) with weights if applicable

    /**
    * Creates a node with the initial values. Exits after displaying an error
    * message if an invalid value is use for node type.
    *
    * @param type : type of this node (value)
    */
    Node(int type);

    /**
    * For an input node, sets the input value which will be the value of a
    * particular attribute
    *
    * @param inputValue
    */
    void setInput(double inputValue);

    /**
    * Calculate the output of a ReLU node using its parents that have already been
    * set. output = input value for an input node and 1 for a bias node.
    */
    void calculateOutput();

    /**
    * Return the sum for this node
    *
    * @return the sum
    */
    double getSum();

    /**
    * Returns the output value for this node.
    *
    * @return the output value
    */
    double getOutput();
};
#endif //NEURAL_NETWORKS_JAVA_VS_C_NODE_HPP
