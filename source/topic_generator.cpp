// Topic_generator.cpp
#include "Topic_generator.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
// #include <jni.h>

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

        // Create a Statement object and add it to Statements
        Statement statement(comment, static_cast<Verdict>(verdict == "true" ? 1 : 0), date);
        statements.addStatement(id++, statement, {});  // Empty topics for now
    }

    std::cout << "Data successfully imported and stored in Statements table." << std::endl;
}

// void Topic_generator::callJavaHelloWorld() {
//     JavaVM* jvm;
//     JNIEnv* env;
//     JavaVMInitArgs vm_args;

//     JavaVMOption options[1];
//     options[0].optionString = (char*)"-Djava.class.path=C:/Users/lucas/GitHub/ISR_code/java";  // Ensure path is correct
//     vm_args.version = JNI_VERSION_1_8;
//     vm_args.nOptions = 1;
//     vm_args.options = options;
//     vm_args.ignoreUnrecognized = false;

//     // Step 1: Create the JVM
//     jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
//     if (res != JNI_OK) {
//         std::cerr << "Failed to create JVM" << std::endl;
//         return;
//     }
//     std::cout << "JVM created successfully." << std::endl;

//     // Step 2: Find the hello_world class
//     jclass helloWorldClass = env->FindClass("hello_world");
//     if (!helloWorldClass) {
//         std::cerr << "Error: Class hello_world not found." << std::endl;
//         jvm->DestroyJavaVM();
//         return;
//     }
//     std::cout << "hello_world class found." << std::endl;

//     // Step 3: Find the sayHello method
//     jmethodID sayHelloMethod = env->GetMethodID(helloWorldClass, "sayHello", "()V");
//     if (!sayHelloMethod) {
//         std::cerr << "Error: Method sayHello() not found." << std::endl;
//         jvm->DestroyJavaVM();
//         return;
//     }
//     std::cout << "sayHello method found." << std::endl;

//     // Step 4: Create an instance of hello_world and call sayHello
//     jobject helloWorldObject = env->NewObject(helloWorldClass, env->GetMethodID(helloWorldClass, "<init>", "()V"));
//     env->CallVoidMethod(helloWorldObject, sayHelloMethod);
//     std::cout << "sayHello method called." << std::endl;

//     // Step 5: Clean up and destroy the JVM
//     jvm->DestroyJavaVM();
//     std::cout << "JVM destroyed." << std::endl;
// }

void Topic_generator::dataDateFilter(const std::string& input) {
    (void)input;
    return;
}

void Topic_generator::buildMalletProfile(const std::string& input, const std::string& output) {
    (void)input;
    (void)output;
    return;
}

void Topic_generator::generateTopics(const std::string& input, int numTopics, const std::string& outputState, const std::string& output) {
    (void)input;
    (void)numTopics;
    (void)outputState;
    (void)output;
    return;
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
