
#ifndef COSINE_SIMILARITY_H
#define COSINE_SIMILARITY_H

#include <vector>
#include <string>

class Similarity {
public:
    double cosineSimilarity(std::vector<std::string> originWord, std::vector<std::string> targetWord);
};

class Values {
public:
    int value1;
    int value2;

    Values();

    Values(int v1, int v2);

    void UpdateValue(int v1, int v2);

    ~Values();
};

inline Values::Values() {}

inline Values::Values(int v1, int v2) {
    this->value1 = v1;
    this->value2 = v2;
}

inline void Values::UpdateValue(int v1, int v2) {
    this->value1 = v1;
    this->value2 = v2;
}

inline Values::~Values() {
    value1 = 0;
    value2 = 0;
}

#endif //COSINE_SIMILARITY_H
