// package cc.mallet.examples;

// import cc.mallet.util.*;
// import cc.mallet.types.*;
// import cc.mallet.pipe.*;
// import cc.mallet.pipe.iterator.*;
// import cc.mallet.topics.*;

// import java.util.*;
// import java.util.regex.*;
// import java.io.*;

// public class topic_model {

//     private ParallelTopicModel model;
//     private int numTopics;
//     private InstanceList instances;

//     public topic_model(int numTopics) throws IOException {
//         this.numTopics = numTopics;

//         ArrayList<Pipe> pipeList = new ArrayList<>();
//         pipeList.add(new CharSequenceLowercase());
//         pipeList.add(new CharSequence2TokenSequence(Pattern.compile("\\p{L}[\\p{L}\\p{P}]+\\p{L}")));
//         pipeList.add(new TokenSequenceRemoveStopwords(new File("stoplists/en.txt"), "UTF-8", false, false, false));
//         pipeList.add(new TokenSequence2FeatureSequence());

//         instances = new InstanceList(new SerialPipes(pipeList));
//     }

//     // Method to add data from JSON file
//     public void addDataFromFile(String filePath) throws IOException {
//         Reader fileReader = new InputStreamReader(new FileInputStream(new File(filePath)), "UTF-8");
//         instances.addThruPipe(new CsvIterator(fileReader, Pattern.compile("^(\\S*)[\\s,]*(\\S*)[\\s,]*(.*)$"), 3, 2, 1));
//     }

//     // Method to add data directly from statements
//     public void addDataFromStatements(List<String> statements) {
//         for (String statement : statements) {
//             instances.addThruPipe(new Instance(statement, null, null, null));
//         }
//     }

//     // Method to train the model and generate topics
//     public void trainModel() throws IOException {
//         model = new ParallelTopicModel(numTopics, 1.0, 0.01);
//         model.addInstances(instances);
//         model.setNumThreads(2);
//         model.setNumIterations(1000);
//         model.estimate();
//     }

//     // Example method to get topic words
//     public List<String> getTopWords(int topic, int numWords) {
//         ArrayList<TreeSet<IDSorter>> topicSortedWords = model.getSortedWords();
//         TreeSet<IDSorter> sortedWords = topicSortedWords.get(topic);

//         List<String> topWords = new ArrayList<>();
//         int rank = 0;
//         for (IDSorter idCountPair : sortedWords) {
//             if (rank++ >= numWords) break;
//             topWords.add(instances.getDataAlphabet().lookupObject(idCountPair.getID()).toString());
//         }
//         return topWords;
//     }
// }
