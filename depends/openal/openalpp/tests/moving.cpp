/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 *  
 * Simple example for using OpenAL++.
 * Plays a file bee.wav and moves it left right. 
 */
#include <cc++/thread.h>
#include <openalpp/alpp.h>
#include <iostream>

using namespace openalpp;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int main() {
  try {
    Source *asd = new Source("bee.wav");
    Source beesound("bee.wav");
	  beesound.SetGain(1);
	  beesound.SetLooping();

    float limits[2] = {5,-15};
    float delay=10;
    float time=0,angle=0;
    
    beesound.SetPosition(limits[0],0.0,0.0);
	  beesound.Play();

    const int no_laps=5;

    std::cerr << "Moving sound 5 laps..." << std::endl;

    // Do a cheat time loop.
    while(angle<(M_PI*2.0*no_laps)) {
      ost::Thread::sleep(delay); // Wait for delay milliseconds

      time +=delay/1000; // Calculate the time in the loop
      angle=M_PI *time;  // What is the resulting angle
      
      // Calculate a new position
      beesound.SetPosition(limits[0]*cos(angle),0.0,limits[1]*sin(angle));
	  }
  } catch(Error e) {
    std::cerr << e << "\n";
    return -1;
  }
  return 0;
}
  
