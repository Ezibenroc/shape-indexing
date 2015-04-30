///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <thread>
#include <mutex>
#include <dirent.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/PGMReader.h>
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "imageCharacterization.h"
#include "imageClass.h"
///////////////////////////////////////////////////////////////////////////////
namespace po = boost::program_options;


using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

// Verify that the given directory contains at least one pgm file.
bool checkDir(string dirname) {
    bool flag = false ;
    struct stat st;
    lstat((char*)dirname.c_str(), &st);
    if(S_ISDIR(st.st_mode)) {
        struct dirent *entry; // sorry for dirent, boost/filesystem was not available on servsls@ens-lyon.fr
        DIR *dp;
        dp = opendir((char*)dirname.c_str());
        while((entry = readdir(dp)) && !flag) {
            if(!((strcmp(".",entry->d_name)==0) || (strcmp("..",entry->d_name)==0))) {
                flag = flag||checkDir((dirname)+"/"+((string)entry->d_name)) ;
            }
        }
        closedir(dp);
        return flag ;
    }
    else {
        unsigned dotPos = dirname.find_last_of(".") + 1 ;
        unsigned slashPos = dirname.find_last_of("/") + 1 ;
        return (dotPos != dirname.npos && dotPos > slashPos && dirname.substr(dotPos) == "pgm") ;
    }
}

vector<string> getDir(string dirname) {
    struct stat st;
    vector<string> subdirs ;
    lstat((char*)dirname.c_str(), &st);
    if(S_ISDIR(st.st_mode)) {
        struct dirent *entry; // sorry for dirent, boost/filesystem was not available on servsls@ens-lyon.fr
        DIR *dp;
        dp = opendir((char*)dirname.c_str());
        while((entry = readdir(dp))) {
            if(!((strcmp(".",entry->d_name)==0) || (strcmp("..",entry->d_name)==0))) {
                subdirs.push_back((dirname)+"/"+((string)entry->d_name)) ;
                if(!checkDir(subdirs.back())) {
                    cerr << "Error: no pgm files in " << dirname << "." << endl ;
                    exit(EXIT_FAILURE) ;
                }
            }
        }
        closedir(dp);
    }
    else {
        cerr << "Error: " << dirname << " is not a directory." << endl ;
        exit(EXIT_FAILURE) ;
    }
    return subdirs ;
}

void computeClasses(vector<string> *directories, unsigned *index, mutex *index_mutex, vector<ImageClass> *classes, mutex *classes_mutex, ostream *out) {
    bool flag = true ;
    int my_index ;
    while(flag) {
        index_mutex->lock() ;
        if(*index < directories->size()) {
            my_index = *index ;
            *index = *index + 1 ;
            cerr << (*directories)[my_index] << endl ;
        }
        else {
            flag = false ;
        }
        index_mutex->unlock() ;
        if(!flag)
            break ;
        ImageClass img((*directories)[my_index]) ;
        classes_mutex->lock() ;
        classes->push_back(img);
        *out << img ;
        classes_mutex->unlock() ;
    }
}

void printTitle(string str) {
    cerr << "\033[1;32m" << str << "\033[0m\n";
}

void missingParam(std::string param) {
  trace.error() <<" Parameter: "<<param<<" is required..";
  trace.info() <<std::endl;
  exit ( 1 );
}

int main(int argc, char *argv[]) {
    string input, output ;
    int nbThreads = 1 ;
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
      ("help,h", "display this message")
      ("nbthread,j", po::value<int>(), "number of threads to use")
      ("input,i", po::value<std::string>(), "input database folder")
      ("output,o", po::value<std::string>(), "output file for the descriptors") ;
    bool parseOK=true;
    po::variables_map vm;
    try{
      po::store(po::parse_command_line(argc, argv, general_opt), vm);

    po::notify(vm);
    if(vm.count("help")||argc<=1|| !parseOK)
      {
        trace.info()<< "Compute the descriptors of the images contained in the given database." <<std::endl << "Basic usage: "<<std::endl
        << "\t imgAddNoise [options] --input <imageImageFolder> --output <outputFile>"<<std::endl
        << general_opt << "\n";
        return 0;
      }
    //Parameters
    if ( ! ( vm.count ( "input" ) ) ) missingParam ( "--input" );
    input = vm["input"].as<std::string>();
    if ( ! ( vm.count ( "output" ) ) ) missingParam ( "--output" );
    output = vm["output"].as<std::string>();
    if (vm.count ( "output" ))
        nbThreads = vm["nbthread"].as<int>();
    }catch(const std::exception& ex){
      trace.info()<< "Error checking program options: "<< ex.what()<< std::endl; // there is an error, but it still seems to work...
    }

    vector<string> directories = getDir(input) ;
    unsigned index = 0 ;
    mutex index_mutex ;
    vector<ImageClass> classes ;
    mutex classes_mutex ;
    vector<thread> threads ;
    std::ofstream outfile(output);
    for(int i = 0 ; i < nbThreads ; i++) {
        threads.push_back(thread(computeClasses, &directories, &index, &index_mutex, &classes, &classes_mutex, &outfile)) ;
    }
    for(int i = 0 ; i < nbThreads ; i++) {
        threads[i].join() ;
    }
    return 0;
}
