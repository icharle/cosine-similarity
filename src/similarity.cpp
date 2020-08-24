
#include "similarity.h"
#include <map>
#include <math.h>

double Similarity::cosineSimilarity(std::vector<std::string> originWord, std::vector<std::string> targetWord) {
    std::map<std::string, Values> wordDict = std::map<std::string, Values>();

    for (size_t i = 0; i < originWord.size(); ++i) {
        std::string tmp_wd = originWord.at(i);
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
    for (size_t i = 0; i < targetWord.size(); ++i) {
        std::string tmp_wd = targetWord.at(i);
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
    double dictNum = 0, originNum = 0, targetNum = 0;
    for (auto it = wordDict.begin(); it != wordDict.end(); it++) {
        int origin = it->second.value1;
        int des = it->second.value2;
        originNum += origin * origin;
        targetNum += des * des;
        dictNum += origin * des;
    }

    return dictNum / sqrt(originNum * targetNum);
}