#include <iostream>
#include <string>
#include <fstream>

#include "../../src/interface.h"

using std::string;

  int main()
  {
	  Comparator comparator;
	  ComparisonReport report;

	  string firstFileName;
	  std::cout << "First file name: \n";
	  std::getline(std::cin, firstFileName);
	  string secondFileName;
	  std::cout << "Second file name: \n";
	  std::getline(std::cin, secondFileName);

	  std::ifstream firstFile(firstFileName);
	  std::ifstream secondFile(secondFileName);

	  if (firstFile.is_open() && secondFile.is_open()) {
		  report = comparator.compare(firstFile, secondFile);
	  }
	  firstFile.close();
	  secondFile.close();

	  std::multiset<std::string> commonWords = report.commonWords.words();
	  std::cout << "Command words are: " << commonWords.size() << std::endl;
	  for (auto word : commonWords) {
		  std::cout << word << " ";
	  }
	  std::cout << '\n';

	  std::multiset<std::string> uniqueWordsForFirstStream = report.uniqueWords[0].words();
	  std::cout << "Unique words for first file are: " << uniqueWordsForFirstStream.size() << std::endl;
	  for (auto word : uniqueWordsForFirstStream) {
		  std::cout << word << " ";
	  }
	  std::cout << '\n';

	  std::multiset<std::string> uniqueWordsForSecondStream = report.uniqueWords[1].words();
	  std::cout << "Unique words for second file are " << uniqueWordsForSecondStream.size() << std::endl;
	  for (auto word : uniqueWordsForSecondStream) {
		  std::cout << word << " ";
	  }
  }