
#include "similarity.h"
#include <map>
#include <math.h>

double Similarity::cosineSimilarity(std::vector<std::string> originWordVector, std::vector<std::string> targetWordVector) {
    std::map<std::string, Values> wordDict = std::map<std::string, Values>();

    // 词向量统计
    for (size_t i = 0; i < originWordVector.size(); ++i) {
        std::string tmp_wd = originWordVector.at(i);
        if (wordDict.find(tmp_wd) != wordDict.end()) {
            // 值+1
            Values value1 = wordDict.at(tmp_wd);
            int freq11 = value1.value1 + 1;
            int freq12 = value1.value2;
            value1.UpdateValue(freq11, freq12);
            wordDict[tmp_wd] = value1;
            value1.~Values();
        } else {
            // 压入1
            Values value1 = Values(1, 0);
            wordDict.emplace(tmp_wd, value1);
            value1.~Values();
        }
    }
    for (size_t i = 0; i < targetWordVector.size(); ++i) {
        std::string tmp_wd = targetWordVector.at(i);
        if (wordDict.find(tmp_wd) != wordDict.end()) {
            // 值+1
            Values value2 = wordDict.at(tmp_wd);
            int freq21 = value2.value1;
            int freq22 = value2.value2 + 1;
            value2.UpdateValue(freq21, freq22);
            wordDict[tmp_wd] = value2;
            value2.~Values();
        } else {
            // 压入1
            Values value2 = Values(0, 1);
            wordDict.emplace(tmp_wd, value2);
            value2.~Values();
        }
    }
    // 计算余弦相似度分数
    double resultVector = 0, originVector = 0, targetVector = 0;
    for (auto it = wordDict.begin(); it != wordDict.end(); it++) {
        int origin = it->second.value1;
        int des = it->second.value2;
        originVector += origin * origin;
        targetVector += des * des;
        resultVector += origin * des;
    }

    return resultVector / sqrt(originVector * targetVector);
}