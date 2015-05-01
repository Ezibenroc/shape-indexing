#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <functional>

int main(int argc, char**argv) {
    //argv[1] is the classification score file
    //argv[2] is the class the image belongs to [1..70]
    std::ifstream myfile (argv[1], std::ifstream::in);
    int realclass = atoi(argv[2]);
    std::vector< std::pair<double,int> > scores;

    int cpt=0;
    double score;
    while ((myfile.good()) && (cpt<70)) {
        myfile >> score;
        scores.push_back( std::make_pair(score,cpt+1));
        ++cpt;
    }

    if (cpt < 70) {
        std::cerr << "Problem in the score file!"<<std::endl;
        exit(2);
    }

    //We just return the range of realclass
    std::sort(scores.begin(), scores.end(), std::greater< std::pair<double,int> >());

    //find the rank (bruteforce)
    unsigned i = 0 ;
    while(i != scores.size() && scores[i].second != realclass)
        i++;

    if (i == scores.size()) {
        std::cerr << "Class not found!"<<std::endl;
        exit(2);
    }

    //exaeco
    score = scores[i].first;
    while(i > 0 && scores[i-1].first == score)
        i-- ;

    //We output the rank
    std::cout << i+1 <<std::endl;
    return 0 ;
}
