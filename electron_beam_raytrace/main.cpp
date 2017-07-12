/*
    main.cpp

    Copyright (C) 2013 by Don Cross  -  http://cosinekitty.com/raytrace

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the author be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
       distribution.

    -------------------------------------------------------------------------

    Main source file for my demo 3D ray-tracing image maker.
*/

#include <iostream>
#include <string>
#include "algebra.h"
#include "imager.h"

void AtomTest()
{
  using namespace Imager;
  //primitive vectors of BCC
  const Vector a(0.5, 0.0, 0.0);
  const Vector b(0.0, 0.5, 0.0);
  const Vector c(0.25, 0.25, 0.25);
  const Vector x1(1.0, 0.0, 0.0);
  const Vector x2(0.0, 1.0, 0.0);
  const Vector x3(0.0, 0.0, -1.0);
  //Number of atoms change here
  double n = 5;

  Scene scene(Color(0.0, 0.0, 0.0));
  Sphere* bigsphere = new Sphere(Vector(0.0,0.0, -30.0), 2*n);
  // bigsphere->SetFullMatte(Color(0.0, 0.1, 0.0));
  scene.AddSolidObject(bigsphere);
  
  for (double i = 0; i < n; i++)
    for(double j = 0; j < n; j++)   
      {
	Sphere*  s0 = new Sphere((c+i*a+j*b), 0.1);
	s0->Translate(-n/4.0, -n/4.0, -8.0);
	scene.AddSolidObject(s0);     
	
	Sphere*  s1 = new Sphere((a+i*a+j*b), 0.1);
	s1->Translate(-n/4.0, -n/4.0, -8.0);			
	scene.AddSolidObject(s1);     

	Sphere*  s2 = new Sphere((b+i*a+j*b), 0.1);
	s2->Translate(-n/4.0, -n/4.0, -8.0);
	scene.AddSolidObject(s2);
      }

  // Add a light source to illuminate the objects in the scene; otherwise we won't see anything!
  scene.AddLightSource(LightSource(Vector(0.0, +30.0, 0.0), Color(1.0, 1.0, 1.0)));
  scene.AddLightSource(LightSource(Vector(0.0, -30.0, 0.0), Color(1.0, 1.0, 1.0)));
  scene.AddLightSource(LightSource(Vector(+30.0, 0.0, 0.0), Color(1.0, 1.0, 1.0)));
  scene.AddLightSource(LightSource(Vector(-30.0, 0.0, 0.0), Color(1.0, 1.0, 1.0)));
  scene.AddLightSource(LightSource(Vector(0.0, 0.0, +30.0), Color(0.1, 1.0, 0.1)));
  scene.AddLightSource(LightSource(Vector(0.0, 0.0, -30.0), Color(0.1, 1.0, 0.1)));
// Generate a PNG file that displays the scene...
  const char *filename = "atoms.png";
  scene.SaveImage(filename, 600, 600, 1.0, 1);
  std::cout << "Wrote " << filename << std::endl;
}



//---------------------------------------------------------------------------
// Define a new type that is a pointer to a function
// with void return type and taking zero arguments.
typedef void (* COMMAND_FUNCTION) ();

struct CommandEntry
{
    const char* verb;           // the command line option
    COMMAND_FUNCTION command;   // function to call when option encountered
    const char* help;           // usage text that explains the option
};

// You can add more command line options to this program by
// adding another entry to the array below.
// Each item in the ray is a string followed by a 
// function to be called when that string appears 
// on the command line.
const CommandEntry CommandTable[] =
{
    { "atoms", AtomTest,
        "    Top of BCC structure.\n"
    },
};

// Calculate the number of entries in CommandTable[]...
const size_t NUM_COMMANDS = sizeof(CommandTable) / sizeof(CommandTable[0]);


void PrintUsageText()
{
    using namespace std;

    cout <<
        "Ray Tracer demo - Copyright (C) 2013 by Don Cross.\n"
        "For more info, see: http://cosinekitty.com/raytrace\n"
        "\n"
        "The following command line options are supported:\n";

    for (size_t i=0; i < NUM_COMMANDS; ++i)
    {
        cout << "\n";
        cout << CommandTable[i].verb << "\n";
        cout << CommandTable[i].help;
    }

    cout << endl;
}


int main(int argc, const char *argv[])
{
    using namespace std;

    int rc = 1;

    if (argc == 1)
    {
        // No command line arguments are present, so display usage text.
        PrintUsageText();
    }
    else
    {
        // There is at least one command line option present.
        // Search the command table for the matching verb.
        const string verb = argv[1];
        bool found = false;     // did we find a matching verb in the table?
        for (size_t i=0; i < NUM_COMMANDS; ++i)
        {
            if (verb == CommandTable[i].verb)
            {
                found = true;                   // we found a match!
                CommandTable[i].command();      // call the function
                rc = 0;                         // indicate success
                break;                          // stop searching
            }
        }

        if (!found)
        {
            cerr << "ERROR:  Unknown command line option '" << verb << "'" << endl;
        }
    }

    return rc;
}
