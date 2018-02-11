/*
    Institution: Tecnologico de Costa Rica
    Course: Digital Image Processing
    Author: Luis Leon Vega
    Last Uptate: Feb/11th/2018

    Code: This is a image loader based on LTIlib-2 Library.

    http://www.boost.org/doc/libs/1_60_0/libs/program_options/example/options_description.cpp
*/

#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <iterator>

#include <ltiViewer2D.h>
#include <ltiIOImage.h>
#include <ltiIOLTI.h>
#include <ltiImage.h>
#include <cstdlib>

using namespace std;


// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
}

// Create object

lti::viewer2D::interaction action;
lti::ipoint pos;

int main(int ac, char* av[])
{
    try {
        int opt;
        int portnum;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("optimization", po::value<int>(&opt)->default_value(10),
                  "optimization level")
            ("verbose,v", po::value<int>()->implicit_value(1),
                  "enable verbosity (optionally specify level)")
            ("listen,l", po::value<int>(&portnum)->implicit_value(1001)
                  ->default_value(0,"no"),
                  "listen on a port.")
            ("include-path,I", po::value< vector<string> >(),
                  "include path")
            ("input-file", po::value< vector<string> >(), "input file")
        ;

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }

        if (vm.count("include-path"))
        {
            cout << "Include paths are: "
                 << vm["include-path"].as< vector<string> >() << "\n";
        }

        if (vm.count("input-file"))
        {
            cout << "Input files are: "
                 << vm["input-file"].as< vector<string> >() << "\n";

            /*
                Executing the image showing
            */       
            lti::image img;
            lti::ioImage loader; 
            vector<string> path = vm["input-file"].as< vector<string> >();
            // Load image
            if(loader.load(path[0],img))
            {
                // Viewer
                static lti::viewer2D view;
                lti::viewer2D::parameters vpar(view.getParameters());
                vpar.title = path[0]; // set the image name in the title bar
                view.setParameters(vpar); 
                view.show(img);  
                bool ok = false;
                do {
                  view.waitInteraction(action,pos); // wait for something to happen
                  if (action == lti::viewer2D::Closed) { // window closed?
                    ok = true;
                  
                  }
                } while(!ok);
            }
            else
            {
                cerr << loader.getStatusString() << std::endl;
                cout << "Error loading image" << "\n";
            }
            
            
            
        }

        if (vm.count("verbose")) {
            cout << "Verbosity enabled.  Level is " << vm["verbose"].as<int>()
                 << "\n";
        }

    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}