#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <thread>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ATimer.h>

#define MAX_NAME_SIZE 256

using namespace std;

std::string path="./fstest/"; 
int64_t writeCount = 100;
int     gap = 100;        
std::vector<std::string> nameVector;
uint64_t bufSize = 4194304;  //4MB block
std::string flatName="./fstest/flat.test";
int fd;

/** 
 * \brief Thread for reading images as individual files
 **/
void readSingleThread( std::vector<std::string> nameVector) 
{
   //Allocate a new buffer
   char buffer[bufSize];

   usleep(500);

   for( int i = 0; i < writeCount; i++ ) {
      int fd = open( nameVector[i].c_str(), O_RDWR  );
      if( fd < 0 ) {
         fprintf(stderr, "%d Unable to open file %s for reading\n"
                , fd
                , nameVector[i].c_str()
                );
         return;
      }
      int count = read( fd, buffer, bufSize );
      if( count < 0 ) {
         fprintf( stderr, "Error reading file\n");
         i--;
      }

      close(fd);
   }

   printf("ReadThread complete\n");
}

/** 
 * \brief Thread for writing images as individual files
 **/
void writeSingleThread( std::vector<std::string> nameVector) 
{
   atl::Timer timer;
   timer.start();

   //Allocate a new buffer
   char buffer[bufSize];

   for( int64_t i = 0; i < writeCount; i++ ) 
   {
      bool openfile = false;
      uint64_t written = 0;
      int fd = open( nameVector[i].c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG  );
      if( fd < 0 ) {
         fprintf(stderr, "%d Unable to open file %s for writing\n"
                , fd
                , nameVector[i].c_str()
                );
      }
      else {
         openfile = true;
      }

      while( openfile &&( written < bufSize )) {

         int bcount = write( fd, buffer, bufSize);
         if( bcount <= 0 ) {
            fprintf(stderr, "%d Unable to write to file %s\n"
                   , fd
                   , nameVector[i].c_str()
                   );
            i--;
            openfile = false;
         }
         else {
            written += bcount;
         }
      }

      close(fd);
      openfile = false;
   }

   double runTime = timer.elapsed();

   printf("%ld %lf files created in %lf seconds\n"
         , writeCount
         , double(bufSize)/(double)1e6
         , runTime
         );
}

/**
 * \brief prints the help 
 **/
void printHelp() {
   printf(" Tests the write throughput to files in a filesystem. This application tests standard files, flat files, and memory mapped files.\n");
   printf("\nUsage:\n");
   printf("\t-d destination directory\n");
   printf("\t-g gap between file written and file read (%d)\n", gap);
   printf("\t-n number of files to write (%ld)\n", writeCount );
   printf("\t-s size of files to write (%ld)\n", bufSize );
   printf("\t-t specific test to run( n=normal file, f=flat file, m=memory map )\n");
   printf("\n");
   printf("Examples:\n");
   printf("To run in default mode: \"./writeTest\"\n");
   printf("To write to /media/data1: \"./writeTest -d /media/data1\"\n");
   printf("To only do a memoryMap and normal test: \"./writeTest -t nm\"\n");
   printf("\n\n");
}


//Test Flags
#define NORMAL_TEST  0x01
#define FLAT_TEST    0x02
#define MAP_TEST     0x04
#define ALL_TEST     0xFF



int main(int argc, char * argv[]) 
{
   printf("\nwriteTest application\n");
   int argCount = 0;
   uint8_t testFlag = ALL_TEST;

   //Read in command line parameters to check read/write solution
   for( int i = 1; i < argc; i++ ) {
      if( !strcmp(argv[i], "-d")) { //d: assign path
         argCount++;
         i++;
         path.assign( argv[i] );
      } 
      else if( !strcmp(argv[i], "-g" )) {
         argCount++;
         i++;
         gap = atoi(argv[i]);
      }
      else if( !strcmp(argv[i], "-n" )) {
         argCount++;
         i++;
         writeCount = atol(argv[i]);
      }
      else if( !strcmp(argv[i], "-s" )) {
         argCount++;
         i++;
         bufSize = atol(argv[i]);
      }
      else if( !strcmp(argv[i], "-t" )) {
         argCount++;
         i++;
         std::string tmpstr = argv[i];
         testFlag = 0;
         if( tmpstr.find('f') != std::string::npos) {
            testFlag |= FLAT_TEST;
         }
         if( tmpstr.find('m') != std::string::npos) {
            testFlag |= MAP_TEST;
         }
         if( tmpstr.find('n') != std::string::npos) {
            testFlag |= NORMAL_TEST;
         }
      }
      else {
         printf("\nUnknown input parameter %d: %s\n", i, argv[i]);
         printHelp();
         exit(1);
      }
   }

   //Set destination path

   std::stringstream ss;
   ss << "rm -rf "<< path.c_str()<<"; mkdir " << path.c_str();
   std::string removeCmd = ss.str();

   //Preallocate a name for all vectors
   nameVector.resize(writeCount);
   for( int64_t i = 0; i < writeCount; i++ )  {
      std::stringstream ss;
      ss << path << i << ".test";
      nameVector[i].assign(ss.str());
   }

   //Ok, we've got the names, now let's write the files
   //
   // Current - warm up
   //
   if( testFlag & NORMAL_TEST) {
      // TODO: why write two times?
      printf("--- Warm up\n");
      writeSingleThread( nameVector );

      //Start read thread .. write? 
      printf("--- Write existing\n");
      writeSingleThread( nameVector );

      // Read and write with existing files
      //
      printf("--- RW existing\n");
      std::thread rt1(readSingleThread, nameVector );
      writeSingleThread( nameVector );
      rt1.join();
   }

   printf("Write test complete!\n");
   return 0;
}
