//
// Created by Haylee on 11/13/2016.
//

#ifndef NEURAL_NETWORKS_JAVA_VS_C_INSTANCE_HPP
#define NEURAL_NETWORKS_JAVA_VS_C_INSTANCE_HPP

#include <vector>

class Instance {
private:
    std::vector<double> attributes;
    std::vector<int> classValues;

public:
    /**
     * Adds an attribute to the attributes field of this instance
     *
     * @param attribute : the attribute to be added
     */
    void addAttribute(double attribute);

    /**
     * Adds a class value to the classValues field of this instance
     *
     * @param classValue : the class value to be added
     */
    void addClassValue(int classValue);

    /**
     * Return the attributes field of this instance
     *
     * @return athe attributes for this instance
     */
    std::vector<double>& getAttributes();

    /**
     * Return the classValues field of this instance
     *
     * @return the class values for this instance
     */
    std::vector<int>& getClassValues();
};

#endif //NEURAL_NETWORKS_JAVA_VS_C_INSTANCE_HPP
