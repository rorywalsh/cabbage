/*
  ==============================================================================

    CabbageCsound.h
    Created: 20 Oct 2016 2:41:24pm
    Author:  rory

  ==============================================================================
*/

#ifndef CABBAGECSOUND_H_INCLUDED
#define CABBAGECSOUND_H_INCLUDED

#include <csound.hpp>

class CabbageCsound : public Csound
{

	public:
		CabbageCsound():Csound()
		{
			
		}
		
		void compileCsdFile(File csdFile)
		{
			csCompileResult = Compile(const_cast<char*>(csdFile.getFullPathName().toUTF8().getAddress()));
		}
		
		bool compiledWithoutError()
		{
			return csCompileResult==0 ? true : false;	
		}

	int csCompileResult=-1;

	
};


#endif  // CABBAGECSOUND_H_INCLUDED
