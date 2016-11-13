/**
 * Class to identfiy connections between different layers.
 */

public class NodeWeightPair{
    public Node node; //The parent node
    public double weight; //Weight of this connection

    // Create an object with a given parent node and connect weight
    public NodeWeightPair(Node node, double weight) {
        NodeWeightPair::node = node;
        NodeWeightPair::weight = weight;
    }
}
