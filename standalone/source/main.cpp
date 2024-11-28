#include <factify/factify.h>
#include <factify/version.h>
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>


#include "Topic_generator.h"
#include "Statements.h"
#include "Network_synthesizer.h"

auto main(int argc, char** argv) -> int {
    const std::unordered_map<std::string, factify::LanguageCode> languages{
        {"en", factify::LanguageCode::EN},
        {"de", factify::LanguageCode::DE},
        {"es", factify::LanguageCode::ES},
        {"fr", factify::LanguageCode::FR},
    };
    cxxopts::Options options(*argv, "A program to welcome the world!");

    std::string language;
    std::string name;

    // clang-format off
    options.add_options()
        ("h,help", "Show help")
        ("v,version", "Print the current version number")
        ("n,name", "Name to greet", cxxopts::value(name)->default_value("World"))
        ("l,lang", "Language code to use", cxxopts::value(language)->default_value("en"));
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result["version"].as<bool>()) {
        std::cout << "Factify, version " << FACTIFY_VERSION << std::endl;
        return 0;
    }

    auto langIt = languages.find(language);
    if (langIt == languages.end()) {
        std::cerr << "unknown language code: " << language << std::endl;
        return 1;
    }

    factify::Factify factify(name);
    std::cout << factify.greet(langIt->second) << std::endl;

    // Create Topic_generator and Statements instances
    Topic_generator topicGenerator;
    Statements statements;

    topicGenerator.importStoreData(statements);
    topicGenerator.buildMalletProfile(statements, "profile1");
    topicGenerator.generateTopics("profile1", 35, "profile1");
    topicGenerator.assignTopics(statements, 35, "profile1");

    Network_Synthesizer networkSynthesizer(statements, 35);
    networkSynthesizer.calculateSimilarity();
    networkSynthesizer.buildNetwork();
    networkSynthesizer.findMST();


    return 0;
}
