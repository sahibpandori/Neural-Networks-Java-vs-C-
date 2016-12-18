#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "Node.hpp"
#include "NodeWeightPair.hpp"
#include "Instance.hpp"
#include "NNImpl.hpp"


std::vector<Instance> getData(std::string);
void readWeights(std::vector<std::vector<double>> &,
                 std::vector<std::vector<double>> &);

int main(int argc, char *argv[]) {
    double startTime = std::time(0);
    double timeTaken;
    //Checking for correct number of arguments
    if (argc < 6) {
        std::cout << "USAGE: " << argv[0] << " <noHiddenNode> ";
        std::cout << "<learningRate> <maxEpoch> <trainFile> <testFile>";
        std::exit(-1);
    }

    //Reading the training set
    std::vector<Instance> trainingSet = getData(std::string(argv[4]));

    //Reading the weights
    std::vector<std::vector<double>> hiddenWeights;
    hiddenWeights.resize((unsigned) std::atoi(argv[1]));

    for(int i = 0; i < hiddenWeights.size(); i++) {
        hiddenWeights[i].resize(trainingSet[0].attributes.size() + 1);
    }

    std::vector<std::vector<double>> outputWeights;
    outputWeights.resize(trainingSet[0].classValues.size());

    for (int i = 0; i < outputWeights.size(); i++) {
        outputWeights[i].resize(hiddenWeights.size() + 1);
    }

    readWeights(hiddenWeights,outputWeights);

    double learningRate = std::stod(argv[2]);

    if (learningRate > 1 || learningRate <= 0) {
        std::cout << "Incorrect value for learning rate" << std::endl;
        std::exit(-1);
    }

    NNImpl nn(trainingSet, atoi(argv[1]), learningRate, atoi(argv[3]),
              hiddenWeights, outputWeights);
    nn.train();

    //Reading the training set
    std::vector<Instance> testSet = getData(std::string(argv[5]));

    int outputs[testSet.size()];


    int correct = 0;
    for (int i = 0; i < testSet.size(); i++) {
        //Getting output from network
        outputs[i] = nn.calculateOutputForInstance(testSet[i]);
        int actual_idx =- 1;
        for (int j = 0; j < testSet[i].classValues.size(); j++) {
            if (testSet[i].classValues[j] > 0.5) {
                actual_idx = j;
            }
        }

        if (outputs[i] == actual_idx) {
            correct++;
        } else {
            std::cout << i << "th instance got an misclassification, ";
            std::cout << "expected: " << actual_idx << ". But actual:";
            std::cout << outputs[i] << std::endl;
        }
    }

    std::cout << "Total instances: " << testSet.size() << std::endl;
    std::cout << "Correctly classified: " << correct << std::endl;

    timeTaken = (std::time(0) - startTime)/1000000;
    std::cout << "Time elapsed: " << timeTaken;
}

// Reads a file and returns the list of instances read
std::vector<Instance> getData(std::string file) {
    std::vector<Instance> data;
    int attributeCount = 0;
    int outputCount = 0;

    try{
        std::ifstream inFile(file.c_str());
        if (!inFile.is_open()) {
            std::cerr << "Input file " << file << " cannot be opened!" << std::endl;
            std::exit(1);
        }

        std::string line, prefix, token;
        std::vector<std::string> vals;
        while (true) {
            std::getline(inFile, line);
            if(inFile.fail()) {
                break;
            }

            prefix = line.substr(0, 2);
            if (!prefix.compare("//")) {
            } else if (!prefix.compare("##")) {
                attributeCount = std::stoi(line.substr(2, line.length()));
            } else if (!prefix.compare("**")) {
                outputCount = std::stoi(line.substr(2, line.length()));
            } else {
                // Split by space
                std::istringstream ss(line);

                vals.clear();
                while (ss >> token) {
                    vals.push_back(token);
                }

                Instance instance;
                for (int i = 0; i < attributeCount; i++) {
                    instance.attributes.push_back(std::stod(vals[i]));
                }
                for (int i=attributeCount; i < vals.size(); i++) {
                    instance.classValues.push_back(std::stoi(vals[i]));
                }
                data.push_back(instance);
            }
        }
        inFile.close();
        return data;
    }catch(std::exception& e) {
        std::cout << "Could not read instances: " << e.what() << std::endl;
    }
    return {};
}

// Assigns random weights in the vectors passed as parameters
void readWeights(std::vector<std::vector<double>> &hiddenWeights,
                 std::vector<std::vector<double>> &outputWeights) {
//    Random r = new Random();
    int i, j;
    for (i = 0; i < hiddenWeights.size(); i++) {
        for (j = 0; j < hiddenWeights[i].size(); j++) {
            hiddenWeights[i][j] = (double) std::rand() * 0.01 / (double) RAND_MAX;
        }
    }

    for (i = 0; i < outputWeights.size(); i++) {
        for (j = 0; j < outputWeights[i].size(); j++) {
            outputWeights[i][j] = (double) std::rand() * 0.01 / (double) RAND_MAX;
        }
    }
}
