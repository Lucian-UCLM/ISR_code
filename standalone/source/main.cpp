#include "topic_generator.h"
#include "statements.h"
#include "network_synthesizer.h"
#include <iostream>
#include <string>

auto main() -> int {

    // Create Topic_generator and Statements instances
    Topic_generator topicGenerator;
    Statements statements;

    topicGenerator.importStoreData(statements);
    topicGenerator.buildMalletProfile(statements, "profile1");
    topicGenerator.generateTopics("profile1", 60, "profile1");
    topicGenerator.perplexityPypelyne(60);
    topicGenerator.assignTopics(statements, 60, "profile1");

    Network_Synthesizer networkSynthesizer(statements, 60);
    networkSynthesizer.calculateSimilarity();
    networkSynthesizer.buildNetwork();
    networkSynthesizer.findMST();

    networkSynthesizer.exportMSTToGraphMLWithNodeData("./temp/mst_with_data.graphml", statements);
    networkSynthesizer.exportMSTWithNodeData("./temp/mst_edges.csv", "./temp/node_data.csv", statements);

    return 0;
}
