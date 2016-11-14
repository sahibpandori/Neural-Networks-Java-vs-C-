
/**
 * Holds data for a particular instance.
 * Attributes are represented as an ArrayList of Doubles
 * Class labels are represented as an ArrayList of Integers. For example,
 * a 3-class instance will have classValues as [0 1 0] meaning this
 * instance has class 1.
 * Do not modify
 */
#include <vector>
#include "Instance.hpp"

void Instance::addAttribute(double attribute) {
    Instance::attributes.push_back(attribute);
}

void Instance::addClassValue(int classValue) {
    Instance::classValues.push_back(classValue);
}

std::vector<double> Instance::getAttributes() {
    return Instance::attributes;
}

std::vector<int> Instance::getClassValues() {
    return Instance::classValues;
}
