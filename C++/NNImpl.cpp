/**
 * The main class that handles the entire network
 * Has multiple attributes each with its own use
 *
 */

#include "Instance.hpp"
#include "NNImpl.hpp"
#include "NodeWeightPair.hpp"

NNImpl::NNImpl(std::vector<Instance> trainingSet,
               int hiddenNodeCount,
               double learningRate,
               int maxEpoch,
               std::vector<std::vector<double>> hiddenWeights,
               std::vector<std::vector<double>> outputWeights) {
    NNImpl::trainingSet = trainingSet;
    NNImpl::learningRate = learningRate;
    NNImpl::maxEpoch = maxEpoch;

    long inputNodeCount = NNImpl::trainingSet[0].getAttributes().size();
    long outputNodeCount = NNImpl::trainingSet[0].getClassValues().size();
    int i, j;
    for (i = 0; i < inputNodeCount; i++) {
        Node node(0);
        NNImpl::inputNodes.push_back(node);
    }

    //bias node from input layer to hidden
    Node biasToHidden(1);
    NNImpl::inputNodes.push_back(biasToHidden);

    //hidden layer nodes
    for (i = 0; i < hiddenNodeCount; i++) {
        Node node(2);
        //Connecting hidden layer nodes with input layer nodes
        for (j = 0; j < NNImpl::inputNodes.size();j++) {
            NodeWeightPair nwp(&this->inputNodes[j], hiddenWeights[i][j]);
            node.addParent(nwp);
        }
        NNImpl::hiddenNodes.push_back(node);
    }

    //bias node from hidden layer to output
    Node biasToOutput(3);
    NNImpl::hiddenNodes.push_back(biasToOutput);

    //Output node layer
    for (i = 0; i < outputNodeCount; i++) {
        Node node(4);
        //Connecting output layer nodes with hidden layer nodes
        for (j = 0; j < NNImpl::hiddenNodes.size(); j++) {
            NodeWeightPair nwp(&NNImpl::hiddenNodes[j], outputWeights[i][j]);
            node.addParent(nwp);
        }
        NNImpl::outputNodes.push_back(node);
    }
}

int NNImpl::calculateOutputForInstance(Instance instance) {
    int i;
    for (i = 0; i < instance.getAttributes().size(); i++) {
        NNImpl::inputNodes[i].setInput(instance.getAttributes()[i]);
    }
    NNImpl::inputNodes[instance.getAttributes().size()].setInput(1.0);

    for (i = 0; i < NNImpl::hiddenNodes.size(); i++) {
        NNImpl::hiddenNodes[i].calculateOutput();
    }
    NNImpl::hiddenNodes[hiddenNodes.size()-1].setInput(1.0);

    for (i = 0; i < NNImpl::outputNodes.size(); i++) {
        NNImpl::outputNodes[i].calculateOutput();
    }

    int classification = 0;
    double maxOutput = NNImpl::outputNodes[0].getOutput();

    for (i = 1; i < NNImpl::outputNodes.size(); i++) {
        if(NNImpl::outputNodes[i].getOutput() > maxOutput) {
            maxOutput = NNImpl::outputNodes[i].getOutput();
            classification = i;
        }
    }
    return classification;
}

void NNImpl::train() {
    int i, j, k;
    Instance instance;
    for(int e = 0; e < NNImpl::maxEpoch; e++) {
        for(i = 0; i < NNImpl::trainingSet.size(); i++) {
            instance = NNImpl::trainingSet[i];
            calculateOutputForInstance(instance);

            double errorOutput[NNImpl::hiddenNodes.size()][NNImpl::outputNodes.size()];
            double errorHidden[NNImpl::inputNodes.size()][NNImpl::hiddenNodes.size()];

            double error[outputNodes.size()];

            for (j = 0; j < outputNodes.size(); j++) {
                if(NNImpl::outputNodes[j].getSum() >= 0) {
                    error[j] = (instance.getClassValues()[j] -
                                NNImpl::outputNodes[j].getOutput());// * outputNodes.get(j).getSum();
                } else {
                    error[j] = 0;
                }
            }

            //Computing error in hidden to output weights
            for (j = 0; j < NNImpl::hiddenNodes.size(); j++) {
                for (k = 0; k < NNImpl::outputNodes.size(); k++) {
                    if (NNImpl::hiddenNodes[j].getSum() > 0) {
                        errorOutput[j][k] =
                                NNImpl::learningRate * NNImpl::hiddenNodes[j].getOutput() * error[k];
                    } else {
                        errorOutput[j][k] = 0;
                    }
                }
            }

            //Computing the error in input to hidden weights
            for(int a = 0; a < NNImpl::inputNodes.size(); a++) {
                for (j = 0; j < NNImpl::hiddenNodes.size(); j++) {
                    double sigma = 0.0;
                    for (k = 0; k < NNImpl::outputNodes.size(); k++) {
                        if (NNImpl::outputNodes[k].getSum() > 0) {
                            sigma += NNImpl::outputNodes[k].getParentAt(j).getWeight() * error[k];
                        }
                    }

                    if (NNImpl::hiddenNodes[j].getSum() > 0) {
                        errorHidden[a][j] =
                                NNImpl::learningRate * NNImpl::inputNodes[i].getOutput() * sigma;//* hiddenNodes.get(j).getSum();
                    } else {
                        errorHidden[a][j] = 0;
                    }
                }
            }

            //Updating all the weights
            for (j  = 0; j < NNImpl::hiddenNodes.size(); j++) {
                for (k = 0; k < NNImpl::outputNodes.size(); k++) {
                    NNImpl::outputNodes[k].getParentAt(j).addWeight(errorOutput[j][k]);
                }
            }

            for (j = 0; j < NNImpl::inputNodes.size(); j++) {
                for (k = 0; k < NNImpl::hiddenNodes.size() - 1; k++) {
                    hiddenNodes[k].getParentAt(j).addWeight(errorHidden[j][k]);
                }
            }
        }
        calculateOutputForInstance(instance);
    }
}
