/*
 * Copyright (C) 2009 by Pablo Alvarado
 * 
 * This file is part of the lecture CE-5201 Digital Image Processing and
 * Analysis, at the Costa Rica Institute of Technology.
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   viewer.cpp
 *         Simple example of viewer2D
 * \author Pablo Alvarado
 * \date   15.08.2009
 * revisions ..: $Id: viewer.cpp,v 1.4 2014-08-21 09:33:21 alvarado Exp $
 */

#include <ltiViewer2D.h>
#include <ltiIOImage.h>
#include <ltiIOLTI.h>
#include <ltiImage.h>
#include <cstdlib>
#include <iostream>
#include <cstdlib>

/*
 * Help 
 */
void usage()  {
  
  
  std::cout <<
    "usage: viewer [options] [<file1> [<file2> ...]]\n\n"        \
    "       -c|--channel     assume given images are channels\n" \
    "       -h|--help        show this help\n"                    \
    "       <file_i>         input images/channels" << std::endl;    
}

int main(int argc,char* argv[]) {

  bool viewChannel=false;

  // let's assume all arguments are images to be displayed
  if (argc > 1) {
    lti::ioImage loader;
    lti::image img;
    lti::channel8 chnl8;
    lti::channel chnl;
    lti::viewer2D::interaction action;
    lti::ipoint pos;
    bool theEnd = false;
    int i=1; // first image
    do {

      if (std::string(argv[i])=="-c") {
        viewChannel=true;
        ++i;
        if (i>=argc) {
          theEnd=true;
          continue;
        }
      } else if (std::string(argv[i])=="-h") {
        usage();
        theEnd=true;
        continue;
      } 

      // try to get some info on the image
      lti::ioImage::headerInformation info;
      bool loadOk=false;
      if (loader.checkHeader(argv[i],info)) {
	if (info.type == "LTI") {
	  lti::ioLTI ltiLoader;
	  // The LTI format can contain many things
	  char c=0;
	  if (!info.info.empty()) {
	    c=info.info[0];
	  }
	  switch(c) {
	  case 'b':
	    loadOk=ltiLoader.load(argv[i],chnl8);
	    chnl.clear();
	    img.clear();
	    break;
	  case 'f':
	    loadOk=ltiLoader.load(argv[i],chnl);
	    chnl8.clear();
	    img.clear();
	    break;
	  default:
	    loadOk=false;
	    chnl8.clear();
	    chnl.clear();
	    img.clear();
	  }
	} else {
	  loadOk=loader.load(argv[i],img);
	  chnl8.clear();
	  chnl.clear();
	}
      }

      // try to read the image
      if (loadOk) {
	
        // reading image successful
        static lti::viewer2D view;
        lti::viewer2D::parameters vpar(view.getParameters());
        vpar.title = argv[i]; // set the image name in the title bar
        view.setParameters(vpar);

	if (!chnl.empty()) {
	  view.show(chnl);
	} else if (!chnl8.empty()) {
	  view.show(chnl8);
	} else if (!img.empty()) {
	  if (viewChannel) {
	    chnl8.castFrom(img);
	    view.show(chnl8);
	  } else {
	    view.show(img); // show the image
	  }
	}
       
        // wait for the user to close the window or to indicate if she wants
        // to go to the next image or to the previous one
        bool ok = false;
        do {
          view.waitInteraction(action,pos); // wait for something to happen
          if (action == lti::viewer2D::Closed) { // window closed?
            theEnd = true; // we are ready here!
            ok = true;
          } else if (action == lti::viewer2D::KeyPressed) { // key pressed?
            switch (action.key) {
            case lti::viewer2D::RightKey:
            case lti::viewer2D::DownKey:
            case lti::viewer2D::PageDownKey:
              i++;
              if (i>= argc) {
                i=1;
              }
              ok = true;
              break;
            case lti::viewer2D::LeftKey:
            case lti::viewer2D::UpKey:
            case lti::viewer2D::PageUpKey:
              i--;
              if (i<1) {
                i=argc-1;
              }
              ok = true;
              break;
            }
          }
        } while(!ok);
      } else {
        // error reading image
        std::cerr << loader.getStatusString() << std::endl;
        ++i;
	if (i>=argc) {
	  if (argc==2) {
	    return EXIT_FAILURE;
	  } 
	  i=argc-1;
	}
      }
    } while (!theEnd);
  } else {
    std::cout << "Provide at least one image file to be displayed. \n" 
              << "Usage: " << argv[0] << " <image1> [<image2> [<image3> ...] ]"
              << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}