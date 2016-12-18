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
               std::vector<std::vector<double>> &hiddenWeights,
               std::vector<std::vector<double>> &outputWeights) {
    this->trainingSet = trainingSet;
    this->learningRate = learningRate;
    this->maxEpoch = maxEpoch;

    long inputNodeCount = this->trainingSet[0].attributes.size();
    long outputNodeCount = this->trainingSet[0].classValues.size();
    int i, j;
    for (i = 0; i < inputNodeCount; i++) {
        Node node(0);
        this->inputNodes.push_back(node);
    }

    //bias node from input layer to hidden
    Node biasToHidden(1);
    this->inputNodes.push_back(biasToHidden);

    //hidden layer nodes
    for (i = 0; i < hiddenNodeCount; i++) {
        Node node(2);
        //Connecting hidden layer nodes with input layer nodes
        for (j = 0; j < this->inputNodes.size(); j++) {
            NodeWeightPair nwp(&this->inputNodes[j], hiddenWeights[i][j]);
            node.parents.push_back(nwp);
        }
        this->hiddenNodes.push_back(node);
    }

    //bias node from hidden layer to output
    Node biasToOutput(3);
    this->hiddenNodes.push_back(biasToOutput);

    //Output node layer
    for (i = 0; i < outputNodeCount; i++) {
        Node node(4);
        //Connecting output layer nodes with hidden layer nodes
        for (j = 0; j < this->hiddenNodes.size(); j++) {
            NodeWeightPair nwp(&this->hiddenNodes[j], outputWeights[i][j]);
            node.parents.push_back(nwp);
        }
        this->outputNodes.push_back(node);
    }
}

int NNImpl::calculateOutputForInstance(Instance &instance) {
    int i;
    for (i = 0; i < instance.attributes.size(); i++) {
        this->inputNodes[i].setInput(instance.attributes[i]);
    }
    this->inputNodes[instance.attributes.size()].setInput(1.0);

    for (i = 0; i < this->hiddenNodes.size(); i++) {
        this->hiddenNodes[i].calculateOutput();
    }
    this->hiddenNodes[hiddenNodes.size()-1].setInput(1.0);

    for (i = 0; i < this->outputNodes.size(); i++) {
        this->outputNodes[i].calculateOutput();
    }

    int classification = 0;
    double maxOutput = this->outputNodes[0].getOutput();

    for (i = 1; i < this->outputNodes.size(); i++) {
        if(this->outputNodes[i].getOutput() > maxOutput) {
            maxOutput = this->outputNodes[i].getOutput();
            classification = i;
        }
    }
    return classification;
}

void NNImpl::train() {
    int i, j, k;
    Instance instance;
    for(int e = 0; e < this->maxEpoch; e++) {
        for(i = 0; i < this->trainingSet.size(); i++) {
            instance = this->trainingSet[i];
            calculateOutputForInstance(instance);

            double errorOutput[this->hiddenNodes.size()][this->outputNodes.size()];
            double errorHidden[this->inputNodes.size()][this->hiddenNodes.size()];

            double error[outputNodes.size()];

            for (j = 0; j < outputNodes.size(); j++) {
                if(this->outputNodes[j].getSum() >= 0) {
                    error[j] = (instance.classValues[j] -
                                this->outputNodes[j].getOutput());// * outputNodes.get(j).getSum();
                } else {
                    error[j] = 0;
                }
            }

            //Computing error in hidden to output weights
            for (j = 0; j < this->hiddenNodes.size(); j++) {
                for (k = 0; k < this->outputNodes.size(); k++) {
                    if (this->hiddenNodes[j].getSum() > 0) {
                        errorOutput[j][k] =
                                this->learningRate * this->hiddenNodes[j].getOutput() * error[k];
                    } else {
                        errorOutput[j][k] = 0;
                    }
                }
            }

            //Computing the error in input to hidden weights
            for(int a = 0; a < this->inputNodes.size(); a++) {
                for (j = 0; j < this->hiddenNodes.size(); j++) {
                    double sigma = 0.0;
                    for (k = 0; k < this->outputNodes.size(); k++) {
                        if (this->outputNodes[k].getSum() > 0) {
                            sigma += this->outputNodes[k].parents[j].weight * error[k];
                        }
                    }

                    if (this->hiddenNodes[j].getSum() > 0) {
                        errorHidden[a][j] =
                                this->learningRate * this->inputNodes[i].getOutput() * sigma;//* hiddenNodes.get(j).getSum();
                    } else {
                        errorHidden[a][j] = 0;
                    }
                }
            }

            //Updating all the weights
            for (j  = 0; j < this->hiddenNodes.size(); j++) {
                for (k = 0; k < this->outputNodes.size(); k++) {
                    this->outputNodes[k].parents[j].weight += errorOutput[j][k];
                }
            }

            for (j = 0; j < this->inputNodes.size(); j++) {
                for (k = 0; k < this->hiddenNodes.size() - 1; k++) {
                    this->hiddenNodes[k].parents[j].weight += errorHidden[j][k];
                }
            }
        }
        calculateOutputForInstance(instance);
    }
}
