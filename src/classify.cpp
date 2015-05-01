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
#include "classifier.h"
///////////////////////////////////////////////////////////////////////////////
namespace po = boost::program_options;


using namespace std;
using namespace DGtal;
using namespace DGtal::Z2i; //We'll only consider Z² digital space on
                //32bit integers

void printTitle(string str) {
    cerr << "\033[1;32m" << str << "\033[0m\n";
}

void missingParam(std::string param) {
  trace.error() <<" Parameter: "<<param<<" is required..";
  trace.info() <<std::endl;
  exit ( 1 );
}

int main(int argc, char *argv[]) {
    string CSVfile, descriptorsfile, imagefile ;
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
      ("help,h", "display this message")
      ("classes,c", po::value<std::string>(), "CSV file listing all the classes")
      ("descriptors,d", po::value<std::string>(), "descriptors file")
      ("image,i", po::value<std::string>(), "the image to classify") ;
    bool parseOK=true;
    po::variables_map vm;
    try{
      po::store(po::parse_command_line(argc, argv, general_opt), vm);

    po::notify(vm);
    if(vm.count("help")||argc<=1|| !parseOK)
      {
        trace.info()<< "Classify the given image into one of the given classes." <<std::endl << general_opt << "\n";
        return 0;
      }
    //Parameters
    if ( ! ( vm.count ( "classes" ) ) ) missingParam ( "--classes" );
    CSVfile = vm["classes"].as<std::string>();
    if ( ! ( vm.count ( "descriptors" ) ) ) missingParam ( "--descriptors" );
    descriptorsfile = vm["descriptors"].as<std::string>();
    if ( ! ( vm.count ( "image" ) ) ) missingParam ( "--image" );
    imagefile = vm["image"].as<std::string>();
    }catch(const std::exception& ex){
      trace.info()<< "Error checking program options: "<< ex.what()<< std::endl; // there is an error, but it still seems to work...
    }

    Classifier classifier(CSVfile, descriptorsfile) ;
    // cerr << "Read classifier, " << classifier.numberClasses() << " classes, " << classifier.numberImages()  << " images with signatures of size " << classifier.numberDescriptors() << "." << endl ;
    ImageClass image(imagefile) ;
    // cerr << "Read classified, size = " << image.size() << endl ;
    classifier.classify(image) ;
    return 0;
}
