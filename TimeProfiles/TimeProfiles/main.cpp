//
//  main.cpp
//  TimeProfiles
//
//  Created by Timothy Place on 8/11/15.
//  Copyright (c) 2015 Jamoma. All rights reserved.
//

#include <iostream>

#include "Jamoma.h"

#include "TTDSP.h"
extern "C" TTErr TTLoadJamomaExtension_EffectsLib(void);
extern "C" TTErr TTLoadJamomaExtension_FilterLib(void);
extern "C" TTErr TTLoadJamomaExtension_GeneratorLib(void);



// Note: you must build Jamoma1 first -- for apples-to-apples comparisons you must build a Deployment version

int main(int argc, const char * argv[])
{
	TTLoadJamomaExtension_EffectsLib();
	TTLoadJamomaExtension_FilterLib();
	TTLoadJamomaExtension_GeneratorLib();
	
	
	
	// First Test: set an attribute 100,000 times
	{
		TTAudioObject	g1("gain");
		Jamoma::Gain	g2;
		MTRand			r;

		auto start1 = std::chrono::steady_clock::now();
		
		for (int i=0; i<1000000; i++)
			g1.set("linearGain", r.rand(1.0));
		
		auto finish1 = std::chrono::steady_clock::now();

		
		auto start2 = std::chrono::steady_clock::now();
		
		for (int i=0; i<1000000; i++)
			g2.gain = r.rand(1.0);
		
		auto finish2 = std::chrono::steady_clock::now();

		std::cout << std::endl;
		std::cout << "Set the (linear) `gain` Parameter of the `Gain` class 1 million times:"  << std::endl;
		std::cout << "Jamoma1 = " <<  std::chrono::duration<double, std::milli>(finish1 - start1).count()
		<< " ms       Jamoma2 = "  <<  std::chrono::duration<double, std::milli>(finish2 - start2).count() << " ms"  << std::endl;
	}
	
	
	// Test: set an attribute 100,000 times with db conversion
	{
		TTAudioObject	g1("gain");
		Jamoma::Gain	g2;
		MTRand			r;
		
		auto start1 = std::chrono::steady_clock::now();
		
		for (int i=0; i<1000000; i++)
			g1.set("gain", r.rand(100.0) - 50.0);
		
		auto finish1 = std::chrono::steady_clock::now();
		
		
		auto start2 = std::chrono::steady_clock::now();
		
		for (int i=0; i<1000000; i++)
			g2.gain = std::make_pair(r.rand(100.0) - 50.0, Jamoma::Unit::Db); ;
		
		auto finish2 = std::chrono::steady_clock::now();
		
		std::cout << std::endl;
		std::cout << "Set the db `gain` Parameter of the `Gain` class 1 million times:"  << std::endl;
		std::cout << "Jamoma1 = " <<  std::chrono::duration<double, std::milli>(finish1 - start1).count()
		<< " ms       Jamoma2 = "  <<  std::chrono::duration<double, std::milli>(finish2 - start2).count() << " ms"  << std::endl;
		
		// Note that with the db conversion Jamoma1 is actually a little faster than Jamoma1 with linear gain!
		// This is because the conversion uses a custom setter which is faster than the default setter in Jamoma1.
	}


	
	
	return 0;
}
