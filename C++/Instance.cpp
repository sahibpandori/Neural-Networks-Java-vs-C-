#include <vector>
/**
 * Holds data for a particular instance.
 * Attributes are represented as an ArrayList of Doubles
 * Class labels are represented as an ArrayList of Integers. For example,
 * a 3-class instance will have classValues as [0 1 0] meaning this
 * instance has class 1.
 * Do not modify
 */


class Instance{
    public std::vector<double> attributes;
    public std::vector<int> classValues;
}
