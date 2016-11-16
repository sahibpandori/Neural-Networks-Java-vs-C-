#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Node.hpp"
#include "NodeWeightPair.hpp"
#include "Instance.hpp"
#include "NNImpl.hpp"


std::vector<Instance> getData(char*);
void readWeights(std::vector<std::vector<double>> &,
                 std::vector<std::vector<double>> &);

int main(int argc, char *argv[]) {
    double startTime = std::time(0);
    double timeTaken;
    //Checking for correct number of arguments
    if (argc < 5) {
        std::cout << "USAGE: " << argv[0] << " <noHiddenNode> ";
        std::cout << "<learningRate> <maxEpoch> <trainFile> <testFile>";
        std::exit(-1);
    }

    //Reading the training set
    std::vector<Instance> trainingSet = getData(argv[3]);

    //Reading the weights
    std::vector<std::vector<double>> hiddenWeights;
    hiddenWeights.resize(std::atoi(argv[0]));
    for(int i = 0; i < hiddenWeights.size(); i++) {
        hiddenWeights[i].resize(trainingSet[0].getAttributes().size()+1);
    }

    std::vector<std::vector<double>> outputWeights;
    outputWeights.resize(trainingSet[0].getClassValues().size());
    for (int i = 0; i < outputWeights.size(); i++) {
        outputWeights[i].resize(hiddenWeights.size() + 1);
    }

    readWeights(hiddenWeights,outputWeights);

    std::istringstream ss(argv[1]);
    double learningRate;
    ss >> learningRate;

    std::cout << learningRate << std::endl;

    if (learningRate > 1 || learningRate <= 0) {
        std::cout << "Incorrect value for learning rate" << std::endl;
        std::exit(-1);
    }

    NNImpl nn(trainingSet, atoi(argv[0]), learningRate,atoi(argv[2]),
              hiddenWeights,outputWeights);
    nn.train();

    //Reading the training set
    std::vector<Instance> testSet = getData(argv[4]);

    int outputs[testSet.size()];


    int correct = 0;
    for (int i = 0; i < testSet.size(); i++) {
        //Getting output from network
        outputs[i] = nn.calculateOutputForInstance(testSet[i]);
        int actual_idx =- 1;
        for (int j = 0; j < testSet[i].getClassValues().size(); j++) {
            if (testSet[i].getClassValues()[j] > 0.5) {
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
std::vector<Instance> getData(char* file) {
    std::vector<Instance> data;
    int attributeCount = 0;
    int outputCount = 0;

    try{
        std::ifstream inFile(file);
        if (!inFile.is_open()) {
            std::cerr << "Input file " << file << " cannot be opened!" << std::endl;
            std::exit(1);
        }

        std::string line, prefix, token;
        std::vector<std::string> vals;
        while (true) {
            getline(inFile, line);
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
                    instance.addAttribute(std::stod(vals[i]));
                }
                for (int i=attributeCount; i < vals.size(); i++) {
                    instance.addClassValue(std::stoi(vals[i]));
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
            hiddenWeights[i][j] = std::rand() * 0.01;
        }
    }

    for (i = 0; i < outputWeights.size(); i++) {
        for (j = 0; j < outputWeights[i].size(); j++) {
            outputWeights[i][j] = std::rand() * 0.01;
        }
    }
}
