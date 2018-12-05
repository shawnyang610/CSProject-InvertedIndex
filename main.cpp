/*
 * CS780 Proj7 Inverted Index
 * Shawn Yang
 *
 * How to run:
 * g++ shawn_yang_proj7.cpp
 * ./a.out input.txt
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

void inverted_index(std::ifstream & infile, std::ofstream &outfile){
    typedef std::vector<std::pair<int, int>> positions;
    std::map<std::string, std::vector<std::pair<int, int>>> posMap;
    std::string word;
    std::string line;
    int lineCount=0;
    int wordBeginPos=0;

    auto save_to_map = [&](std::string word, int lineCount, int charCount){
        auto found = posMap.find(word);
        // if word doesnt exist in map, add new key
        if (found == posMap.end()){
            positions newPos;
            newPos.push_back(std::pair<int,int>(lineCount,charCount));
            posMap[word] = newPos;
        }
        // if word exists, push it on existing vector
        else{
            posMap[word].push_back(std::pair<int,int>(lineCount,charCount));
        }
    };

    auto print_n_output=[&](std::pair<std::string, positions> word_iter){
        std::cout<<word_iter.first<<" "<<word_iter.second.size()<<" [";
        outfile<<word_iter.first<<" "<<word_iter.second.size()<<" [";

        for (auto p : word_iter.second){
            std::cout<<"("<<p.first<<","<<p.second<<")";
            outfile<<"("<<p.first<<","<<p.second<<")";

            // output , only if it's not the last
            if (p != word_iter.second[word_iter.second.size()-1]){
                std::cout<<",";
                outfile<<",";
            }
        }
        std::cout<<"]"<<std::endl;
        outfile<<"]"<<std::endl;
    };

    // read each line from input and add each word in map
    while (getline(infile, line)) {
        // for each character in the line
        for (int i=0; i<line.length();i++){
            // if char is alphabet or ', cancatenate word to it
            if (isalpha(line[i]) || line[i]=='\''){
                // reset wordBeginPos
                if (word.length()==0){
                    wordBeginPos = i;
                }
                word = word+line[i];
            }
            else{
                if (word.length()>0){
                    // save each complete word to map along with its lineCount and charCount
                    save_to_map(word,lineCount,wordBeginPos);
                    // reset word to blank for a new word in next iteration
                    word="";
                }
            }
        }
        lineCount++;
    }

    infile.close();

    for (auto word_iter : posMap){
        print_n_output(word_iter);
    }
}


int main(int argc, char** argv) {
    // create infile, outfile stream objects
    std::ifstream infile;
    std::ofstream outfile;
    infile.open(argv[1], std::ios::binary);
    outfile.open("output.txt");
    if (argc<2){
        std::cout<<"Please supply a .txt input file"<<std::endl;
    }

    // call main logic
    inverted_index(infile, outfile);

    // close infile, outfile stream objects
    infile.close();
    outfile.close();

    return 0;
}