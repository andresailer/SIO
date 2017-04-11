#include "lcio.h"

#include "UTIL/LCSplitWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "EVENT/LCIO.h"


#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std ;
using namespace lcio ;


/** Little tool that splits large LCIO files 
 */

// class for processing run and event records 
class RunEventProcessor : public LCRunListener, public LCEventListener{
  
protected:
  LCWriter* lcWrt{NULL} ;
  int nEvent{0} ;
  
public:
  
  RunEventProcessor(const RunEventProcessor&) = delete ;
  RunEventProcessor operator=(const RunEventProcessor&) = delete ;

  RunEventProcessor(const char* outFileName, int splitSize) : nEvent(0) {
    
    // open outputfile
    lcWrt =  new LCSplitWriter( LCFactory::getInstance()->createLCWriter(), splitSize ) ;
    
    try{ lcWrt->open( outFileName ) ; } 
    
    catch(IOException& e){
      cout << "[RunEventProcessor()] Can't open file for writing -  " 
	   << e.what()  << endl ;
      exit(1) ;
    }
    
  }
  
  ~RunEventProcessor(){
    // close outputfile
    lcWrt->close()  ;
    cout << endl << "      "  << nEvent << " events copied ! " << endl ; 
  }
  
  void modifyEvent( LCEvent * /*evt*/ ) {  /*no changes to event ! */ ; }
  
  void processEvent( LCEvent * evt ) {  
    
    // just copy events to outputfiles  
    lcWrt->writeEvent( evt ) ;
    nEvent ++ ;
    //     cout << "         event: " << evt->getEventNumber() 
    // 	 << " [run: " << evt->getRunNumber() << "] copied" << endl ;
  }

  void modifyRunHeader(LCRunHeader* /*run*/){  /*no changes to event ! */ ;}

  // don't manipulate run headers - use analyze 
  void processRunHeader( LCRunHeader* run){

    // just copy run headers to the outputfile
    lcWrt->writeRunHeader( run ) ;
//     cout << "run : " << run->getRunNumber() << " [" 
// 	 << run->getDescription() << "] - header copied " << endl ;
  }

} ;

//=============================================================================

int main(int argc, char** argv ){
  
  char* inFileName  ;
  char* outFileName ;

  try{ // a large try block for debugging ....
    
    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    if( argc != 4 ){
      cout << " usage: lcio_splitfile infilename outfilename sizeInBytes \n" 
	   << "      e.g.:   lcio_split_file simjob.slcio splitjob.slcio 200000 \n" 
	   << endl ;
      exit(1)  ;
    }
    // read file names from command line 
    inFileName = argv[1] ;
    outFileName  = argv[2] ;
    std::stringstream ss( argv[3] ) ;
    int splitSize ;
    ss >> splitSize  ;

    try{  lcReader->open( inFileName ) ; } 
    
    catch( IOException& e){
      cout << "Can't open file : " << e.what()  << endl ;
      exit(1) ;
    }
    
    // create a new RunEventProcessor, register it with the reader
    // and read and proccess the whole stream 
    {
      RunEventProcessor evtProc( outFileName, splitSize )  ;
      
      lcReader->registerLCRunListener( &evtProc ) ; 
      lcReader->registerLCEventListener( &evtProc ) ; 
      
      lcReader->readStream() ;
    } 
    
    lcReader->close() ;

  }

  catch(exception& ex){
    cout << "something went wrong:  " << ex.what() << endl ; 
  }

  return 0 ;
  
}

//=============================================================================

