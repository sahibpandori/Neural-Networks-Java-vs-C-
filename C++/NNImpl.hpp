//
// Created by Haylee on 11/13/2016.
//

#ifndef NEURAL_NETWORKS_JAVA_VS_C_NNIMPL_HPP
#define NEURAL_NETWORKS_JAVA_VS_C_NNIMPL_HPP

#include <vector>
#include "Instance.hpp"
#include "Node.hpp"

class NNImpl {
private:
    std::vector<Node> inputNodes;//list of the output layer nodes.
    std::vector<Node> hiddenNodes;//list of the hidden layer nodes
    std::Vector<Node> outputNodes;// list of the output layer nodes

    std::vector<Instance> trainingSet;//the training set
    double learningRate = 1.0; // variable to store the learning rate
    int maxEpoch = 1; // variable to store the maximum number of epochs

public:
    /**
    * This constructor creates the nodes necessary for the neural network
    * Also connects the nodes of different layers
    * After calling the constructor the last node of both inputNodes and
    * hiddenNodes will be bias nodes.
    *
    * @param trainingSet : the training data
    * @param hiddenNodeCount : number of hidden nodes
    * @param learningRate : the learning rate
    * @param maxEpoch : the maximum number of epochs
    * @param hiddenWeights : weights of the hidden nodes
    * @param outputWeights : weights of the output nodes
    */
    NNImpl(std::vector<Instance> trainingSet,
           int hiddenNodeCount,
           double learningRate,
           int maxEpoch,
           double hiddenWeights[][],
           double outputWeights[][]);

    /**
    * Get the output from the neural network for a single instance
    * Return the idx with highest output values. For example if the outputs
    * of the outputNodes are [0.1, 0.5, 0.2], it should return 1.
    * The parameter is a single instance
    *
    * @param instance : the instance whose output is to be calculated
    * @return the output for this instance
    */
    int calculateOutputForInstance(Instance instance);

    /**
    * Train the neural network using the values stored in the fields
    */
    void train();
};
#endif //NEURAL_NETWORKS_JAVA_VS_C_NNIMPL_HPP
