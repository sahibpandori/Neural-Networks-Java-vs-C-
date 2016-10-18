/**
 * The main class that handles the entire network
 * Has multiple attributes each with its own use
 * 
 */

import java.util.*;


public class NNImpl{
	public ArrayList<Node> inputNodes=null;//list of the output layer nodes.
	public ArrayList<Node> hiddenNodes=null;//list of the hidden layer nodes
	public ArrayList<Node> outputNodes=null;// list of the output layer nodes

	public ArrayList<Instance> trainingSet=null;//the training set

	Double learningRate=1.0; // variable to store the learning rate
	int maxEpoch=1; // variable to store the maximum number of epochs

	/**
	 * This constructor creates the nodes necessary for the neural network
	 * Also connects the nodes of different layers
	 * After calling the constructor the last node of both inputNodes and  
	 * hiddenNodes will be bias nodes. 
	 */

	public NNImpl(ArrayList<Instance> trainingSet, int hiddenNodeCount, Double learningRate, int maxEpoch, Double [][]hiddenWeights, Double[][] outputWeights)
	{
		this.trainingSet=trainingSet;
		this.learningRate=learningRate;
		this.maxEpoch=maxEpoch;

		//input layer nodes
		inputNodes=new ArrayList<Node>();
		int inputNodeCount=trainingSet.get(0).attributes.size();
		int outputNodeCount=trainingSet.get(0).classValues.size();
		for(int i=0;i<inputNodeCount;i++)
		{
			Node node=new Node(0);
			inputNodes.add(node);
		}

		//bias node from input layer to hidden
		Node biasToHidden=new Node(1);
		inputNodes.add(biasToHidden);

		//hidden layer nodes
		hiddenNodes=new ArrayList<Node> ();
		for(int i=0;i<hiddenNodeCount;i++)
		{
			Node node=new Node(2);
			//Connecting hidden layer nodes with input layer nodes
			for(int j=0;j<inputNodes.size();j++)
			{
				NodeWeightPair nwp=new NodeWeightPair(inputNodes.get(j),hiddenWeights[i][j]);
				node.parents.add(nwp);
			}
			hiddenNodes.add(node);
		}

		//bias node from hidden layer to output
		Node biasToOutput=new Node(3);
		hiddenNodes.add(biasToOutput);

		//Output node layer
		outputNodes=new ArrayList<Node> ();
		for(int i=0;i<outputNodeCount;i++)
		{
			Node node=new Node(4);
			//Connecting output layer nodes with hidden layer nodes
			for(int j=0;j<hiddenNodes.size();j++)
			{
				NodeWeightPair nwp=new NodeWeightPair(hiddenNodes.get(j), outputWeights[i][j]);
				node.parents.add(nwp);
			}	
			outputNodes.add(node);
		}	
	}

	/**
	 * Get the output from the neural network for a single instance
	 * Return the idx with highest output values. For example if the outputs
	 * of the outputNodes are [0.1, 0.5, 0.2], it should return 1.
	 * The parameter is a single instance
	 */

	public int calculateOutputForInstance(Instance inst)
	{
		for(int i = 0; i < inst.attributes.size(); i++)
		{
			inputNodes.get(i).setInput(inst.attributes.get(i));
		}
		inputNodes.get(inst.attributes.size()).setInput(1.0);

		for(int i = 0; i < hiddenNodes.size(); i++)
		{
			hiddenNodes.get(i).calculateOutput();
		}
		hiddenNodes.get(hiddenNodes.size()-1).setInput(1.0);

		for(int i = 0; i < outputNodes.size(); i++)
		{
			outputNodes.get(i).calculateOutput();
		}

		int classification = 0;
		double maxOutput = outputNodes.get(0).getOutput();

		for(int i = 1; i < outputNodes.size(); i++)
		{
			if(outputNodes.get(i).getOutput() > maxOutput)
			{
				maxOutput = outputNodes.get(i).getOutput();
				classification = i;
			}
		}

		return classification;
	}





	/**
	 * Train the neural networks with the given parameters
	 * 
	 * The parameters are stored as attributes of this class
	 */

	public void train()
	{
		Instance inst = null;
		for(int e = 0; e < maxEpoch; e++)
		{
			for(int i = 0; i < trainingSet.size(); i++)
			{
				inst = trainingSet.get(i);
				calculateOutputForInstance(inst);
				
				double[][] errorOutput = new double[hiddenNodes.size()][outputNodes.size()];
				double[][] errorHidden = new double[inputNodes.size()][hiddenNodes.size()];

				double[] error = new double[outputNodes.size()];

				for(int j = 0; j < error.length; j++)
				{
					if(outputNodes.get(j).getSum() >= 0)
						error[j] = (inst.classValues.get(j) - outputNodes.get(j).getOutput());// * outputNodes.get(j).getSum();
					else
						error[j] = 0;
				}

				//Computing error in hidden to output weights
				for(int j = 0; j < hiddenNodes.size(); j++)
				{
					for(int k = 0; k < outputNodes.size(); k++)
					{
						if(hiddenNodes.get(j).getSum() > 0)
							errorOutput[j][k] = learningRate * hiddenNodes.get(j).getOutput() * error[k];
						else
							errorOutput[j][k] = 0;
					}
				}

				//Computing the error in input to hidden weights
				for(int a = 0; a < inputNodes.size(); a++)
				{
					for(int j = 0; j < hiddenNodes.size(); j++)
					{
						double sigma = 0.0;
						for(int k = 0; k < outputNodes.size(); k++)
						{
							if(outputNodes.get(k).getSum() > 0)
								sigma += outputNodes.get(k).parents.get(j).weight * error[k];
						}

						if(hiddenNodes.get(j).getSum() > 0)
							errorHidden[a][j] = learningRate * inputNodes.get(a).getOutput() * sigma ;//* hiddenNodes.get(j).getSum();
						else
							errorHidden[a][j] = 0;
					}
				}

				//Updating all the weights
				for(int j = 0; j < hiddenNodes.size(); j++)
				{
					for(int k = 0; k < outputNodes.size(); k++)
					{
						outputNodes.get(k).parents.get(j).weight += errorOutput[j][k];
					}
				}

				for(int j = 0; j < inputNodes.size(); j++)
				{
					for(int k = 0; k < hiddenNodes.size() - 1; k++)
					{
						hiddenNodes.get(k).parents.get(j).weight += errorHidden[j][k];
					}
				}
			}
			calculateOutputForInstance(inst);
		}

	}
}
