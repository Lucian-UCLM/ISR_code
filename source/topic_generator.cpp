// Topic_generator.cpp
#include "Topic_generator.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Topic_generator::Topic_generator() : malletFile(""), rawData("") {
    // Constructor initialization
}

void Topic_generator::importStoreData(Statements& statements) {
    std::ifstream file("input/politifact_factcheck_data_cleaned.json");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file." << std::endl;
        return;
    }

    json data;
    file >> data;

    int id = 0;
    for (const auto& item : data["statements"]) {
        std::string verdict = item["verdict"];
        std::string comment = item["statement"];
        std::string date = item["statement_date"];

        Statement statement(comment, static_cast<Verdict>(verdict == "true" ? 1 : 0), date);
        statements.addStatement(id++, statement, {});
    }

    std::cout << "Data successfully imported and stored in Statements table." << std::endl;
}

void Topic_generator::dataDateFilter(const std::string& input) {
    (void)input;
    return;
}

void Topic_generator::buildMalletProfile(Statements& statements, const std::string& output) {
    std::cout << "Building Mallet profile from Statements table..." << std::endl;

    const std::string tempInputFile = "./temp/statements_only.txt";
    std::ofstream outFile(tempInputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create temporary input file." << std::endl;
        return;
    }

    for (int id = 0; id < statements.getSize(); ++id) {
        outFile << statements.getComment(id) << std::endl;
    }
    outFile.close();
    std::cout << "Statements written to " << tempInputFile << std::endl;

    std::string command = "mallet import-file --input " + tempInputFile + 
                            " --output ./temp/" + output + ".mallet" +
                            " --keep-sequence --remove-stopwords";

    int ret_code = std::system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error: Mallet process failed with exit code " << ret_code << std::endl;
    } else {
        std::cout << "Mallet profile built successfully." << std::endl;
    }
}

void Topic_generator::generateTopics(const std::string& input, int numTopics, const std::string& output) {
    std::cout << "Generating topics..." << std::endl;

    std::string command = "mallet train-topics --input ./temp/" + input + ".mallet" + 
                " --num-topics " + std::to_string(numTopics) +
                " --output-topic-keys ./temp/" + output + "_keys.txt" +
                " --output-doc-topics ./temp/" + output + "_composition.txt" +
                " --diagnostics-file ./temp/" + output + "_diagnostics.xml" +
                " > NUL 2>&1";

    int ret_code = std::system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error: Mallet process failed with exit code " << ret_code << std::endl;
    } else {
        std::cout << "Topics generated successfully." << std::endl;
    }
}

std::vector<DocumentTopic> Topic_generator::parseDocumentTopicComposition(const std::string& filePath) {
    std::vector<DocumentTopic> docTopics;
    std::ifstream file("./temp/" + filePath + "_composition.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return docTopics;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        DocumentTopic docTopic;
        ss >> docTopic.documentId; // First column is document ID

        std::string name;
        ss >> name; // Second column is document name (so ignore it)

        double proportion;
        while (ss >> proportion) {
            docTopic.topicProportions.push_back(proportion);
        }
        docTopics.push_back(docTopic);
    }

    return docTopics;
}

std::vector<TopicWord> Topic_generator::parseTopicWordDistributions(const std::string& filePath) {
    std::vector<TopicWord> topics;
    std::ifstream file("./temp/" + filePath + "_keys.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return topics;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        TopicWord topicWord;
        ss >> topicWord.topicId;

        double weight;
        ss >> weight;

        std::string word;
        while (ss >> word) {
            topicWord.wordProportions[word] += 1.0;
        }
        topics.push_back(topicWord);
    }

    return topics;
}

double Topic_generator::computePerplexity(
    const std::vector<DocumentTopic>& docTopics,
    const std::vector<TopicWord>& topicWords,
    const std::unordered_map<std::string, int>& wordCounts) {

    double logLikelihood = 0.0;
    int totalWords = 0;

    for (const auto& [word, count] : wordCounts) {
        double wordProb = 0.0;

        for (const auto& docTopic : docTopics) {
            for (size_t topicId = 0; topicId < docTopic.topicProportions.size(); ++topicId) {
                auto it = topicWords[topicId].wordProportions.find(word);
                if (it != topicWords[topicId].wordProportions.end()) {
                    wordProb += docTopic.topicProportions[topicId] * it->second;
                }
            }
        }

        if (wordProb > 0) {
            logLikelihood += count * std::log(wordProb);
        }

        totalWords += count;
    }

    return std::exp(-logLikelihood / totalWords);
}

void Topic_generator::calculateAndPrintPerplexity() {
    auto docTopics = parseDocumentTopicComposition("profile1");
    auto topicWords = parseTopicWordDistributions("profile1");

    std::unordered_map<std::string, int> wordCounts = {
        {"president", 10}, {"city", 5}, {"court", 3}, {"tax", 7}
    };

    double perplexity = computePerplexity(docTopics, topicWords, wordCounts);
    std::cout << "Model Perplexity: " << perplexity << std::endl;
}

void Topic_generator::numTopicsSelector(int numOfTopics) {
    (void)numOfTopics;
    return;
}

void Topic_generator::exportSimilarity(const std::string& input, const std::string& fieldsToClean) {
    (void)input;
    (void)fieldsToClean;
    return;
}


// #include <jni.h>

// void Topic_generator::callJavaHelloWorld() {

//     JavaVM* jvm;
//     JNIEnv* env;
//     JavaVMInitArgs vm_args;
//     JavaVMOption options[1];

//     options[0].optionString = (char*)"-Djava.class.path=./java";
//     vm_args.version = JNI_VERSION_1_8;
//     vm_args.nOptions = 1;
//     vm_args.options = options;
//     vm_args.ignoreUnrecognized = false;

//     jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
//     if (res != JNI_OK) {
//         std::cerr << "Failed to create JVM. Error code: " << res << std::endl;
//         return;
//     }
//     std::cout << "JVM created successfully." << std::endl;

//     jclass helloWorldClass = env->FindClass("hello_world");
//     if (!helloWorldClass) {
//         std::cerr << "Error: Class hello_world not found." << std::endl;
//         jvm->DestroyJavaVM();
//         return;
//     }
//     std::cout << "hello_world class found." << std::endl;

//     jmethodID sayHelloMethod = env->GetStaticMethodID(helloWorldClass, "sayHello", "()V");
//     if (!sayHelloMethod) {
//         std::cerr << "Error: Method sayHello() not found." << std::endl;
//         jvm->DestroyJavaVM();
//         return;
//     }
//     std::cout << "sayHello method found." << std::endl;

//     env->CallStaticVoidMethod(helloWorldClass, sayHelloMethod);
//     std::cout << "sayHello method called from Java." << std::endl;

//     jvm->DestroyJavaVM();
//     std::cout << "JVM destroyed." << std::endl;
// }