

 #include <boost/program_options.hpp>
 #include "detector.cpp"

 // Namespaces
 namespace po = boost::program_options;

 // A helper function to simplify the main part.
 template<class T>
 std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
 {
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
 }

 /**
  * @function main
  */
 int main( int ac, const char* av[] ) {
   try {
        //Getting the command parameter

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("cascade,c", po::value<std::string>()->default_value(cascade_name), "It is the cascade.xml file")
            ("minNeighbors,n", po::value<int>()->default_value(minNeighbors), "It the minimum neighbors that should be detected")
            ("input-file,i", po::value< std::vector<std::string> >(), "input file Usage: ./detector -i picture1 picture2 [...]")
        ;

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
        po::notify(vm);

        /*
            Help message
        */
        if (vm.count("help")) {
            std::cout << "Usage: ./detector -c cascade.xml - i picture1 picture2 [...]\n";
            std::cout << "       ./detector picture1 picture2 [...]\n";
            std::cout << "       ./detector -i picture1 picture2 [...]\n";
            std::cout << desc;
            return 0;
        }

        if(vm.count("input-file")){ // Load images
            std::vector<std::string> path = vm["input-file"].as< std::vector<std::string> >();

            // Load cascade
            cascade_name = vm["cascade"].as<cv::String>();
            minNeighbors = vm["minNeighbors"].as<int>();

            //-- 1. Load the cascade
            if( !cascade.load( cascade_name )){
              printf("Error: error loading cascade file\n");
              return -1;
            }

            if(0 < path.size()) { //-- 2. Load the pictures
                // Load each one
                for(int i = 0; i < path.size(); i++) {
                    cv::Mat image;
                    // Load each image and Read the file
                    image = cv::imread(path[i], CV_LOAD_IMAGE_COLOR);

                    if(! image.data ) { // Check for invalid input
                        std::cout <<  "Error: Could not open or find the image" << "\n" ;
                        return -1;
                    }
                    detectAndDisplay( image );//-- 3. Apply detector
                }
                std::cout <<  "Error: Could not open or find the image" << "\n" ;
            }
        }
        else  { // Not images loaded
            std::cout <<  "Error: No images were selected" << "\n" ;
        }
  }
  // Error Handling
  catch(std::exception& e)
  {
      std::cout << e.what() << "\n";
      return 1;
  }
  return 0;

}
