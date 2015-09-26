/***************************************************************************************************
 *
 *  Filename    : checkEvtTool.h
 *  Description : All-in-one class for parsing Goldon JSON File and checking against event
 *  Details     :
 *
***************************************************************************************************/
#ifndef __CHECKEVTTOOL_H__
#define __CHECKEVTTOOL_H__

#include "iostream"
#include "fstream"
#include "string"
#include <map>

class checkEvtTool {
public:
   checkEvtTool( bool debug = false );
   checkEvtTool( std::string myJson, bool debug = false );
   ~checkEvtTool() {}

   void addJson( std::string myJson );
   void makeJsonMap( bool savejson = false, std::string saveAs = "myJSON.txt" );
   void saveJson( std::string saveAs = "myJSON.txt" );
   void listMyJsons();
   void listGoodLumis();

   bool isGoodEvt( int runNumber, int LumiSec );

private:
   bool hasJson;
   bool hasJsonMap;
   bool deBug;
   std::vector<std::string> myJsons;
   std::multimap<int, std::pair<int, int>> JsonMap;    // <runNumber, pair<startLumiSec, endLumiSec> >, multimap for same runNumber

   void checkChars( char* nameInput, std::string& nameOutput, int& status );
};

//-------------------------------------------------------------------------------------------------- 
//   Constructors
//-------------------------------------------------------------------------------------------------- 
checkEvtTool::checkEvtTool( bool debug )
{
   deBug      = debug;
   hasJson    = false;
   hasJsonMap = false;
}

checkEvtTool::checkEvtTool( std::string myJson, bool debug )
{
   deBug      = debug;
   hasJsonMap = false;
   addJson( myJson );
}

//-------------------------------------------------------------------------------------------------- 
//   Public member functions
//-------------------------------------------------------------------------------------------------- 
void checkEvtTool::addJson( std::string myJson )
{
   hasJson = true;
   myJsons.push_back( myJson );
}

void checkEvtTool::makeJsonMap( bool savejson, std::string saveAs )
{
   if( !hasJson ) {
      printf( ">> [ERROR] No any JSON input.\n" );
      printf( "           Please use checkEvt::addJson( std::string &myJson ) to add your JSON file.\n" );
      exit( 0 );
   }
   if( deBug ) { listMyJsons(); }

   char name[128];
   int runNumber = 0;
   int startLumiSec = 0;
   int endLumiSec = 0;
   int status = 0 ;
   for( std::vector<std::string>::iterator ijson = myJsons.begin(); ijson != myJsons.end(); ++ijson ) {
      ifstream JSON( ijson->c_str() );
      if( !JSON ) {
         printf( ">> [ERROR] Can not found JSON file, %s. Please check if the JSON file exists.\n", ijson->c_str() );
         exit( 0 );
      }

      while( !JSON.eof() ) {
         JSON >> name;
         std::string nameOutput;
         checkChars( name, nameOutput, status );
         if( status == 1 ) {
            runNumber = atoi( nameOutput.c_str() );
         } else if( status == 2 ) {
            startLumiSec = atoi( nameOutput.c_str() );
         } else if( status == 3 ) {
            endLumiSec = atoi( nameOutput.c_str() );
            JsonMap.insert( std::make_pair( runNumber, std::make_pair( startLumiSec, endLumiSec ) ) );
         }
      }
      hasJsonMap = true;
      JSON.close();
   }
   if( deBug ) { listGoodLumis(); }
   if( savejson ) { saveJson( saveAs ); }
}

void checkEvtTool::saveJson( std::string saveAs )
{
   if( !hasJsonMap ) {
      printf( ">> [ERROR] Can not save JSON file. Please do checkEvtTool::makeJsonMap() first.\n" );
      exit( 0 );
   }
   if( deBug ) {
      printf( ">> [DEBUG] Call checkEvtTool::saveJson( std::string saveAs ) to save JSON:\n" );
      printf( "           %s\n", saveAs.c_str() );
   }

   int runNumTmp = 0;
   fstream out;
   out.open( saveAs.c_str(), std::ios_base::out );
   out << "{";
   for( std::map<int, std::pair<int, int>>::iterator imap = JsonMap.begin(); imap != JsonMap.end(); ++imap ) {
      int runNum       = imap->first;
      int startLumiSec = imap->second.first;
      int endLumiSec   = imap->second.second;
      if( runNumTmp != runNum ) {
         if( runNumTmp != 0 ) { out << "], "; }
         runNumTmp = runNum;
         out << "\"" << runNum << "\": [[" << startLumiSec << ", " << endLumiSec << "]";
      } else
      {   out << ", [" << startLumiSec << ", " << endLumiSec << "]";}
   }
   out << "]}";
   out.close();
}

void checkEvtTool::listMyJsons()
{
   if( !hasJson ) {
      printf( ">> [ERROR] No any JSON input.\n" );
      printf( "           Please use checkEvt::addJson( std::string &myJson ) to add your JSON file.\n" );
      exit( 0 );
   }

   printf( ">> [DEBUG] Input Json files:\n" );
   for( std::vector<std::string>::iterator ijson = myJsons.begin(); ijson != myJsons.end(); ++ijson ) {
      printf( "           %s\n", ijson->c_str() );
   }
}

void checkEvtTool::listGoodLumis()
{
   if( !hasJsonMap ) {
      printf( "[ERROR] Can not list good lumis. Please do checkEvtTool::makeJsonMap() first. \n" );
      exit( 0 );
   }

   printf( ">> [DEBUG] Total Good Run:[LumiStart, lumiEnd]:\n" );
   for( std::map<int, std::pair<int, int>>::iterator imap = JsonMap.begin(); imap != JsonMap.end(); ++imap ) {
      printf( "%17d:[%4d, %4d]\n", imap->first, imap->second.first, imap->second.second );
   }
}

bool checkEvtTool::isGoodEvt( int runNumber, int LumiSec )
{
   bool isGoodEvt_ = false;
   std::multimap<int, std::pair<int, int>>::iterator JsonMapItr_;
   JsonMapItr_ = JsonMap.find( runNumber );

   if( JsonMapItr_ == JsonMap.end() ) {
      isGoodEvt_ = false;
   } else {
      while ( JsonMapItr_ != JsonMap.end() ) { // loop for same runNumber with different of lumi sections
         if( JsonMapItr_->first != runNumber ) { break; }
         if( LumiSec >= JsonMapItr_->second.first &&
             LumiSec <= JsonMapItr_->second.second ) {
            isGoodEvt_ = true;
         }
         if( isGoodEvt_ ) { break; }
         ++JsonMapItr_;
      }
   }
   return isGoodEvt_;
}

//-------------------------------------------------------------------------------------------------- 
//   Private Member function : JSON file parsing
//-------------------------------------------------------------------------------------------------- 
void checkEvtTool::checkChars( char* nameInput, std::string& nameOutput, int& status )
{
   for( unsigned int size_ = 0; size_ < strlen( nameInput ); size_++ ) {
      if( nameInput[0] == '{' || nameInput[0] == '"' ) {
         status = 1; // for run number
      } else if( nameInput[0] == '[' ) {
         status = 2; // for start evt number
      } else {
         status = 3; // for end evt number
      }
      if( !( nameInput[size_] == '{' || nameInput[size_] == '"' || nameInput[size_] == '[' ||
             nameInput[size_] == '}' || nameInput[size_] == ',' || nameInput[size_] == ']' ||
             nameInput[size_] == ':' ) ) {
         nameOutput += nameInput[size_];
      }
   }
}

#endif

