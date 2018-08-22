#include <iostream>
#include <math.h>

using namespace std;

struct node {
    double z;
    double val;
    double w[2];
    double b;
    double rsd;
};

class net {
public:
    node input_layer[2];
    node hidden_layer[2];
    node output_layer[2];

    double avar = 0.5;
    double Y[2];

    net();
    double sigmoid(double z);
    double sigmoidD(double z);
    double getLoss();
    void forwardProgation(double input1, double input2);
    void backwardProgation(double Y1, double Y2);
    void printResult();
};

net::net() {
    input_layer[0].w[0] = 0.15;
    input_layer[0].w[1] = 0.25;
    input_layer[1].w[0] = 0.20;
    input_layer[1].w[1] = 0.30;
    hidden_layer[0].w[0] = 0.40;
    hidden_layer[0].w[1] = 0.50;
    hidden_layer[0].b = 0.25;
    hidden_layer[1].w[0] = 0.45;
    hidden_layer[1].w[1] = 0.55;
    hidden_layer[1].b = 0.35;
    output_layer[0].b = 0.30;
    output_layer[1].b = 0.40;
}

double net::sigmoid(double z) {
    return 1 / (1 + exp(-z));
}

double net::sigmoidD(double z) {
    return sigmoid(z) * (1 - sigmoid(z));
}

double net::getLoss() {
    return (pow(Y[0] - output_layer[0].val, 2) + pow(Y[1] - output_layer[1].val, 2) / 2);
}

void net::forwardProgation(double input1, double input2) {
    input_layer[0].val = input1;
    input_layer[1].val = input2;
    for (size_t hNNum = 0; hNNum < 2; ++hNNum) {
        double z = 0;
        for (size_t iNNum = 0; iNNum < 2; ++iNNum)
            z += input_layer[iNNum].val * input_layer[iNNum].w[hNNum];
        z += hidden_layer[hNNum].b;
        hidden_layer[hNNum].z = z;
        hidden_layer[hNNum].val = sigmoid(z);
    }
    for (size_t oNNum = 0; oNNum < 2; ++oNNum) {
        double z = 0;
        for (size_t hNNum = 0; hNNum < 2; ++hNNum)
            z += hidden_layer[hNNum].val * hidden_layer[hNNum].w[oNNum];
        z += output_layer[oNNum].b;
        output_layer[oNNum].z = z;
        output_layer[oNNum].val = sigmoid(z);
    }
}

void net::backwardProgation(double Y1, double Y2) {
    Y[0] = Y1;
    Y[1] = Y2;
    for (size_t oNNum = 0; oNNum < 2; ++oNNum)
        output_layer[oNNum].rsd = -(Y[oNNum] - output_layer[oNNum].val) * sigmoidD(output_layer[oNNum].z);
    for (size_t hNNum = 0; hNNum < 2; ++hNNum) {
        double tmp = 0;
        for (size_t oNNum = 0; oNNum < 2; ++oNNum)
            tmp += hidden_layer[hNNum].w[oNNum] * output_layer[oNNum].rsd;
        hidden_layer[hNNum].rsd = tmp * sigmoidD(hidden_layer[hNNum].z);
    }
    for (size_t hNNum = 0; hNNum < 2; ++hNNum) {
        for (size_t oNNum = 0; oNNum < 2; ++oNNum) {
            hidden_layer[hNNum].w[oNNum] -= hidden_layer[hNNum].val * output_layer[oNNum].rsd;
            output_layer[hNNum].b -= output_layer[oNNum].rsd;
        }
    }
    for (size_t iNNum = 0; iNNum < 2; ++iNNum) {
        for (size_t hNNum = 0; hNNum < 2; ++hNNum) {
            input_layer[iNNum].w[hNNum] -= input_layer[iNNum].val * hidden_layer[hNNum].rsd;
            hidden_layer[iNNum].b -= hidden_layer[hNNum].rsd;
        }
    }
}

void net::printResult() {
    cout << "loss : " << getLoss() << endl;
    cout << "output1 : " << output_layer[0].val << endl;
    cout << "output2 : " << output_layer[1].val << endl;
}

void Test() {
    net mnet;
    for (size_t i = 0; i < 10000; i++) {
        mnet.forwardProgation(0.05, 0.1);
        mnet.backwardProgation(0.01, 0.99);
        if (i % 1000 == 0)
            mnet.printResult();
    }
}
