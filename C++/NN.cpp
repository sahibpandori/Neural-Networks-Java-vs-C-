#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<Instance> getData(char*);
void readWeights(std::vector<std::vector<double>> &,
                 std::vector<std::vector<double>> &);

int main(int argc, char *argv[]) {
    double startTime = std::time(0);
    double timeTaken;
    //Checking for correct number of arguments
    if (argsc < 5) {
        std::cout << "USAGE: " << argv[0] << " <noHiddenNode> ";
        std::cout << "<learningRate> <maxEpoch> <trainFile> <testFile>";
        std::exit(-1);
    }

    //Reading the training set
    std::vector<Instance> trainingSet = getData(args[3]);

    //Reading the weights
    std::vector<std::vector<double>> hiddenWeights;
    hiddenWeights.resize(std::atoi(args[0]));
    for(int i = 0; i < hiddenWeights.length; i++) {
        hiddenWeights[i].resize(trainingSet[0].getAttributes.size()+1);
    }

    std::vector<std::vector<double>> outputWeights;
    outputWeights.resize(trainingSet[0].getClassValues.size());
    for (int i=0; i<outputWeights.length; i++) {
        outputWeights[i].resize(hiddenWeights.length+1);
    }

    readWeights(hiddenWeights,outputWeights);

    istringstream ss(args[1]);
    double learningRate;
    ss >> learningRate;

    std::cout << learningRate << std::endl;

    if (learningRate > 1 || learningRate <= 0) {
        std::cout << "Incorrect value for learning rate" << std::endl;
        std::exit(-1);
    }

    NNImpl nn(trainingSet, atoi(args[0]), learningRate,atoi(args[2]),
              hiddenWeights,outputWeights);
    nn.train();

    //Reading the training set
    std::vector<Instance> testSet = getData(args[4]);

    Integer outputs[testSet.size()];


    int correct=0;
    for (int i = 0; i < testSet.size(); i++) {
        //Getting output from network
        outputs[i] = nn.calculateOutputForInstance(testSet[i]);
        int actual_idx =- 1;
        for (int j = 0; j < testSet[i].getGlassValues.size(); j++) {
            if (testSet[i].getClassValues()[j] > 0.5) {
                actual_idx = j;
            }
        }

        if (outputs[i] == actual_idx) {
            correct++;
        } else {
            std::cout << i << "th instance got an misclassification, ";
            std::cout << "expected: " + actual_idx + ". But actual:";
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
    BufferedReader in;
    int attributeCount = 0;
    int outputCount = 0;

    try{
        in = new BufferedReader(new FileReader(file));
        std::ifstream inFile(file);
        if (!inFile.is_open()) {
            std::cerr << "Input file " << argv[1] << " cannot be opened!" << std::endl;
            return 1;
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
                attributeCount = std::stoi(line.substring(2, line.length()));
            } else if (!prefix.compare("**")) {
                outputCount = std::stoi(line.substring(2, line.length()));
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
                for (int i=attributeCount; i < vals.length; i++) {
                    instance.addClassValue(std::stoi(vals[i]));
                }
                data.add(inst);
            }
        }
        inFile.close();
        return data;
    }catch(exception& e) {
        std::cout << "Could not read instances: " << e.what() << std::endl;
    }
    return null;
}

// Assigns random weights in the vectors passed as parameters
void readWeights(std::vector<std::vector<double>> &hiddenWeights,
                 std::vector<std::vector<double>> &outputWeights) {
//    Random r = new Random();
    int i, j;
    for (i = 0; i < hiddenWeights.length; i++) {
        for (j = 0; j < hiddenWeights[i].length; j++) {
            hiddenWeights[i][j] = std::rand() * 0.01;
        }
    }

    for (i = 0; i < outputWeights.length; i++) {
        for (j = 0; j < outputWeights[i].length; j++) {
            outputWeights[i][j] = std::rand() * 0.01;
        }
    }
}