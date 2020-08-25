#include <string>
#include <iostream>
#include "lac.h"
#include "similarity.h"

using namespace std;

int main(int argc, char *argv[]) {
    string model_path = "you model path";
    string dict_path = "";
    if (argc > 1) {
        model_path = argv[1];
    }
    if (argc > 2) {
        dict_path = argv[2];
    }

    // 装载模型和用户词典
    LAC lac(model_path);
    if (dict_path.length() > 1) {
        lac.load_customization(dict_path);
    }

    std::string inputWord;
    cout << "please input the first text: ";
    getline(cin, inputWord);
    auto originWordItem = lac.run(inputWord);
    std::vector<std::string> originWord;
    for (size_t i = 0; i < originWordItem.size(); ++i) {
        originWord.emplace_back(originWordItem[i].word);
    }
    cout << "please input the second text: ";
    getline(cin, inputWord);
    auto targetWordItem = lac.run(inputWord);
    std::vector<std::string> targetWord;
    for (size_t i = 0; i < originWordItem.size(); ++i) {
        targetWord.emplace_back(targetWordItem[i].word);
    }

    Similarity similarity;
    cout << "similar score：" << similarity.cosineSimilarity(originWord, targetWord) << endl;
}