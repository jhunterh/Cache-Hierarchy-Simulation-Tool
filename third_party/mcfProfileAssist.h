/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2005 Intel Corporation 
All rights reserved. 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */

//mcfTraceAssist.h
//Contains globals and utility procedures

// Bugs and criticisms can be directed to
// myers.albert@gmail.com

//KNOB types used to parse tool parameters
KNOB<std::string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", 
      "o", "mcfProfile.out", "specify trace output file name");
KNOB<BOOL> TraceIsOn(KNOB_MODE_WRITEONCE, "pintool",
	"trace", "0", "Tracing is turned on or off (default turned off)");
// Most of the flags (a, c, d, f) are valid when tracing is on
KNOB<BOOL> ASCII(KNOB_MODE_WRITEONCE, "pintool", 
      "a", "0", "use ASCII output file isntead of binary");
KNOB<UINT64> Skip(KNOB_MODE_WRITEONCE,"pintool", 
      "s", "0", "Begin emitting branch descriptors after executing a specified number of instructions ");
KNOB<UINT64> Length(KNOB_MODE_WRITEONCE, "pintool", 
      "l", "0", "Number of instructions to profile (default is no limit)");
KNOB<std::string> Compression(KNOB_MODE_WRITEONCE, "pintool", 
      "c", "0", "Compress trace. Supports bzip2, pbzip2, gzip, and pigz");
KNOB<BOOL> AnnotateDisassembly(KNOB_MODE_WRITEONCE, "pintool", 
      "d", "0", "Annotate descriptors with disassembly (only works when output is ASCII");
KNOB<UINT64> FileLimit(KNOB_MODE_WRITEONCE, "pintool", 
      "f", "50000", "Output file size limit in MB. Tracing will end after reaching this limit. Default is 50000 MB");
KNOB<BOOL> Dynamic(KNOB_MODE_WRITEONCE, "pintool",
	  "dynamic", "0", "Dumping the characteristics periodically. Default is off");
KNOB<UINT64> PeriodOfIns(KNOB_MODE_WRITEONCE, "pintool",
	  "n", "100000", "Period (number of instructions) for dumping the characteristics. Default is 100000 instructions (works only with dynamic analysis)");
KNOB<UINT32> appThreads(KNOB_MODE_WRITEONCE, "pintool",
	  "t", "1", "Total number of application threads. Should mension number of threads for multithreaded application when using dynamic analysis. Default 1");

//holds the number of threads
UINT8 numThreads;
TLS_KEY tls_key;
// Holds the maximum number of possible application threads
UINT8 MaxThreads = 32;

/* Mounika edit start */
volatile BOOL IsProcessExiting = FALSE;
volatile BOOL IsEndOfApplication = FALSE;
//holds actual threadids and assigned thread ids
std::map<THREADID, int>  threadIDs;
/* Mounika edit end */

//get local thread id
UINT8 get_localtid(THREADID threadid)
{
   /* Mounika edit start */
   UINT8 localtid = threadIDs[threadid];
   /* Mounika edit end */
   return localtid;
}

//protects descriptor table
PIN_LOCK table_lock;
//protects xed decoder
PIN_LOCK decoder_lock;
//protects streams
PIN_LOCK cout_lock;
//protects statistics globals
PIN_LOCK count_lock;
//temperory lock for debugging 
PIN_LOCK temp_lock;

//Globals for file
struct tm stamp;
std::string fileName;
std::string statFileName;
//Used to find pin tool overhead time.
timeval t1, t2;

//ofstream for raw files
std::ofstream OutFile;
//ofstream for statistics
std::ofstream StatFile;
//ofstream for dumping periodic statistics
std::ofstream *DynamicStatFile;
//FILE for piping to compression utility
FILE *outPipe;

//output file size limit;
UINT64 fileCount;
UINT64 fileLimit;
//number of instructions to skip;
UINT64  fastforwardLength; 
//number of instructions to profile
UINT64 traceLength;
//number of instructions profile in current period for each thread
std::vector<UINT64> traceLengthInCurrentPeriod;
//if a length isn't specififed
BOOL noLength;
//if using compression
BOOL usingCompression;

//enumeration for branch types
typedef enum : UINT8
{
   // Unconditional Indirect Taken
   UnconditionalIndirectTaken = 0,
   // DNE Unconditional Indirect Not Taken (Unconditional -> Taken)
   // Unconditional Direct Taken
   UnconditionalDirectTaken = 1,
   // DNE Unconditional Direct Not Taken (Unconditional -> Taken)
   // DNE Conditional Indirect Taken (x86,x86_64 doesn't have conditional indirect branches)
   // DNE Conditional Indirect Not Taken (x86,x86_64 doesn't have conditional indirect branches)
   // Conditional Direct Taken 
   ConditionalDirectTaken = 2,
   // Conditional Direct Not Taken
   ConditionalDirectNotTaken = 3,
   //number of types
} BranchTypes;

//Total number of branches
//automatically initialized to zero
std::vector<UINT64> numTotalBranches;

//records number of branches for each type
//only four types supported currently
//global arrays are initialized to zero by default
struct countBranches{
	UINT64 Total;
	UINT64 UnconditionalIndirectTaken;
	UINT64 UnconditionalDirectTaken;
	UINT64 ConditionalDirectTaken;
	UINT64 ConditionalDirectNotTaken;
} branches;

std::vector<countBranches> numBranches;
//counting loads and stores periodically
std::vector<countBranches> PeriodicNumBranches;

//struct used when writing binary file
typedef struct{
   //logical thread id
   UINT8 tid;
   //address of brnach instruction   
   ADDRINT branchAddress;
   //address of target
   ADDRINT targetAddress;
   //BranchType enum to record addressing mode, conditionality, and whether or not it is taken  
   BranchTypes branchType;

} BinaryDescriptorTableEntry;
//size of BinaryDescriptorTableEntry;
UINT8 BinDescriptorTableEntrySize = sizeof(UINT8) + sizeof(ADDRINT)*2 + sizeof(BranchTypes);

//Deques used to hold branch descriptors
std::deque<BinaryDescriptorTableEntry> binDescriptorTable;
std::deque<string> ASCIIDescriptorTable;

//write ASCII descriptor table to file
inline VOID WriteASCIIDescriptorTableToFile(const THREADID threadid)
{
    PIN_GetLock(&table_lock, threadid+1);
   //write contents to disk
   while( !ASCIIDescriptorTable.empty() )
   {
      OutFile << ASCIIDescriptorTable.front();
      ASCIIDescriptorTable.pop_front();
   }
    PIN_ReleaseLock(&table_lock);
}
//write ASCII descriptor table to compression program
inline VOID WriteASCIIDescriptorTableToCompressor(const THREADID threadid)
{
    PIN_GetLock(&table_lock, threadid+1);
   //write contents to disk
   while( !ASCIIDescriptorTable.empty() )
   {
      fputs(ASCIIDescriptorTable.front().c_str(), outPipe);
      ASCIIDescriptorTable.pop_front();
   }
    PIN_ReleaseLock(&table_lock);
}
//write binary descriptor table to file
inline VOID WriteBinaryDescriptorTableToFile(const THREADID threadid)
{
    PIN_GetLock(&table_lock, threadid+1);
   while( !binDescriptorTable.empty() ) 
   {
      BinaryDescriptorTableEntry temp = binDescriptorTable.front();
      OutFile.write((char *)&temp.tid, sizeof(temp.tid));
      OutFile.write((char *)&temp.branchAddress, sizeof(temp.branchAddress));
      OutFile.write((char *)&temp.targetAddress, sizeof(temp.targetAddress));
      OutFile.write((char *)&temp.branchType, sizeof(temp.branchType));
      binDescriptorTable.pop_front();
   }
    PIN_ReleaseLock(&table_lock);
}
//write binary descriptor table to compression program
inline VOID WriteBinaryDescriptorTableToCompressor(const THREADID threadid)
{
    PIN_GetLock(&table_lock, threadid+1);
   while( !binDescriptorTable.empty() ) 
   {
      BinaryDescriptorTableEntry temp = binDescriptorTable.front();
      fwrite(&temp.tid, sizeof(temp.tid), 1,outPipe);
      fwrite(&temp.branchAddress, sizeof(temp.branchAddress), 1, outPipe);
      fwrite(&temp.targetAddress, sizeof(temp.targetAddress), 1, outPipe);
      fwrite(&temp.branchType, sizeof(temp.branchType), 1, outPipe);
      binDescriptorTable.pop_front();
   }
    PIN_ReleaseLock(&table_lock);
}
//Worker ASCII write thread when not using compression
VOID ThreadWriteASCII(VOID *arg)
{
   THREADID threadid = PIN_ThreadId();
   if(usingCompression)
   {
      while(1)
      {
         //if process is closing (entered fini()) kill thread
		/* Mounika edit start*/
         if( IsProcessExiting )
            PIN_ExitThread(1);
		 /* Mounika edit end*/
         WriteASCIIDescriptorTableToCompressor(threadid);
      }
   }
   else
   {
      while(1)
      {
         //if process is closing (entered fini()) kill thread
		/* Mounika edit start*/
         if( IsProcessExiting )
            PIN_ExitThread(1);
		 /* Mounika edit end*/
         WriteASCIIDescriptorTableToFile(threadid);
      }
   }
}

//Worker bin write thread when not using compression
VOID ThreadWriteBin(VOID *arg)
{
   THREADID threadid = PIN_ThreadId();
   if(usingCompression)
   {
      while(1)
      {
         //if process is closing (entered fini()) kill thread
		/* Mounika edit start*/
         if( IsProcessExiting )
            PIN_ExitThread(1);
		 /* Mounika edit end*/
         WriteBinaryDescriptorTableToCompressor(threadid);
      }
   }
   else
   {
      while(1)
      {
         //if process is closing (entered fini()) kill thread
		 /* Mounika edit start*/
         if( IsProcessExiting )
            PIN_ExitThread(1);
		 /* Mounika edit end*/
         WriteBinaryDescriptorTableToFile(threadid);
      }
   }
}

//Used to check fast forward and trace length in analysis functions
inline BOOL CanEmit(const THREADID threadid)
{
    PIN_GetLock(&count_lock, threadid+1);
   if( fastforwardLength <  Skip.Value() )
   {
       PIN_ReleaseLock(&count_lock);
      return false;
   }
   else
   {
       PIN_ReleaseLock(&count_lock);
      return true;
   }
}

//check file size and exit early if too large
inline VOID IncrementFileCount(UINT32 size)
{
   //increment counter
   fileCount += size;
   //if limit reached, exit
   if(fileCount >= fileLimit)
      PIN_Detach();
}

//Used to get the size of a file
/* Mounika edit start */
inline int getFileSize(const std::string &fileName)
{
	ifstream file(fileName.c_str(), ifstream::in | ifstream::binary);

	if (!file.is_open())
	{
		return -1;
	}

	file.seekg(0, ios::end);
	int fileSize = file.tellg();
	file.close();

	return fileSize;
}
/* Mounika edit end */

//Address width
//extended
#if defined(TARGET_IA32E)
   #define ADDRWID 16
//32 bit
#else
   #define ADDRWID 8
#endif
//ins contains the instruction string
inline VOID getAssemblyString(std::string & ins, const THREADID threadid, const ADDRINT address)
{
   
    PIN_GetLock(&decoder_lock, threadid+1);

   //Use Intel XED to decode the instruction based on its address
#if defined(TARGET_IA32E)
    static const xed_state_t dstate = {XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b};
#else
    static const xed_state_t dstate = {XED_MACHINE_MODE_LEGACY_32, XED_ADDRESS_WIDTH_32b};
#endif
   xed_decoded_inst_t xedd;
   xed_decoded_inst_zero_set_mode(&xedd,&dstate);

   UINT32 max_inst_len = 15;

   xed_error_enum_t xed_code = xed_decode(&xedd, reinterpret_cast<UINT8*>(address), max_inst_len);
   BOOL xed_ok = (xed_code == XED_ERROR_NONE);
   if (xed_ok) {
      char buf[2048];
      xed_uint64_t runtime_address = static_cast<xed_uint64_t>(address);
      xed_decoded_inst_dump_xed_format(&xedd, buf, 2048, runtime_address);

      ins.assign(buf);
   }
   else
      ins = "XED failed to decode this instruction";

    PIN_ReleaseLock(&decoder_lock);
} 

inline void IncrementEachThreadBranchStatistics(const BranchTypes branchType, UINT8 id)
{
	// Update the counters
	numBranches[id].Total++;
	PeriodicNumBranches[id].Total++;
	switch (branchType)
	{
	case UnconditionalIndirectTaken:
		numBranches[id].UnconditionalIndirectTaken++;
		PeriodicNumBranches[id].UnconditionalIndirectTaken++;
		break;
	case UnconditionalDirectTaken:
		numBranches[id].UnconditionalDirectTaken++;
		PeriodicNumBranches[id].UnconditionalDirectTaken++;
		break;
	case ConditionalDirectTaken:
		numBranches[id].ConditionalDirectTaken++;
		PeriodicNumBranches[id].ConditionalDirectTaken++;
		break;
	case ConditionalDirectNotTaken:
		numBranches[id].ConditionalDirectNotTaken++;
		PeriodicNumBranches[id].ConditionalDirectNotTaken++;
		break;
	default: break;
	}
}

//return percentage
inline double Percentage(const UINT64 num, const UINT64 den)
{
   if( den == 0 )
      return std::numeric_limits<double>::quiet_NaN();

   double ratio = (double)num/(double)den;
   return 100*ratio;
}

VOID PrintStatistics()
{
   
   double elapsedTime;
   elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      
   elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   

   StatFile << "Instrumentation Time: " <<  elapsedTime << " ms" << std::endl;
   StatFile << "Number of Threads: " << static_cast<UINT32>(numThreads) << std::endl;
   StatFile << "Traced " << traceLength+1 << " instructions" << std::endl;
   StatFile << "Skipped " << fastforwardLength << " instructions" << std::endl;  

   //StatFile << "Recorded " << numTotalBranches << " control transfer instructions." << std::endl;

   branches.Total = branches.UnconditionalIndirectTaken = branches.UnconditionalDirectTaken = branches.ConditionalDirectTaken = branches.ConditionalDirectNotTaken = 0;
   for (int i = 0; i < numThreads; i++){
	   branches.Total += numBranches[i].Total;
	   branches.UnconditionalIndirectTaken += numBranches[i].UnconditionalIndirectTaken;
	   branches.UnconditionalDirectTaken += numBranches[i].UnconditionalDirectTaken;
	   branches.ConditionalDirectTaken += numBranches[i].ConditionalDirectTaken;
	   branches.ConditionalDirectNotTaken += numBranches[i].ConditionalDirectNotTaken;
   }
   StatFile << "All The Threads Total Control Transfer Instructions: " << branches.Total << std::endl;
   StatFile.setf(std::ios::fixed);
   StatFile << "\t" << branches.ConditionalDirectTaken << " ( %" << std::setprecision(2)
	   << Percentage(branches.ConditionalDirectTaken, branches.Total) << " ) Conditional Direct Taken" << std::endl;

   StatFile << "\t" << branches.ConditionalDirectNotTaken << " ( %" << std::setprecision(2)
	   << Percentage(branches.ConditionalDirectNotTaken, branches.Total) << " ) Conditional Direct Not Taken" << std::endl;

   StatFile << "\t" << branches.UnconditionalDirectTaken << " ( %" << std::setprecision(2)
	   << Percentage(branches.UnconditionalDirectTaken, branches.Total) << " ) Unconditional Direct" << std::endl;

   StatFile << "\t" << branches.UnconditionalIndirectTaken << " ( %" << std::setprecision(2)
	   << Percentage(branches.UnconditionalIndirectTaken, branches.Total) << " ) Unconditional Indirect" << std::endl;

   for (int i = 0; i < numThreads; i++){
	   StatFile << "   Thread " << i << " Total Control Transfer Instructions: " << numBranches[i].Total << std::endl;
	   StatFile << "\t" << numBranches[i].ConditionalDirectTaken << " ( %" << std::setprecision(2)
		   << Percentage(numBranches[i].ConditionalDirectTaken, numBranches[i].Total) << " ) Conditional Direct Taken" << std::endl;

	   StatFile << "\t" << numBranches[i].ConditionalDirectNotTaken << " ( %" << std::setprecision(2)
		   << Percentage(numBranches[i].ConditionalDirectNotTaken, numBranches[i].Total) << " ) Conditional Direct Not Taken" << std::endl;

	   StatFile << "\t" << numBranches[i].UnconditionalDirectTaken << " ( %" << std::setprecision(2)
		   << Percentage(numBranches[i].UnconditionalDirectTaken, numBranches[i].Total) << " ) Unconditional Direct" << std::endl;

	   StatFile << "\t" << numBranches[i].UnconditionalIndirectTaken << " ( %" << std::setprecision(2)
		   << Percentage(numBranches[i].UnconditionalIndirectTaken, numBranches[i].Total) << " ) Unconditional Indirect" << std::endl;
   }
   //Albert
   //used for benchmark characterization
   StatFile << static_cast<UINT32>(numThreads) << "," << traceLength << "," 
			<< branches.Total << ","
			<< branches.ConditionalDirectTaken + branches.ConditionalDirectNotTaken << ","
			<< branches.UnconditionalDirectTaken << ","
			<< branches.UnconditionalIndirectTaken << std::endl;

   //File Size
   /* Mounika edit start */
   StatFile << getFileSize(statFileName) << std::endl;
   /* Mounika edit end*/
   StatFile << static_cast<UINT32>(numThreads) << "," << elapsedTime << std::endl;
}

/* Mounika edit start */
VOID PrintDynamicStatistics(UINT8 i, UINT64 CurrentPeriod)
{
	DynamicStatFile[i] << (UINT16)i << ", " << CurrentPeriod << ", " << PeriodicNumBranches[i].Total << ", " << PeriodicNumBranches[i].ConditionalDirectTaken
		<< ", " << PeriodicNumBranches[i].ConditionalDirectNotTaken << ", " << PeriodicNumBranches[i].UnconditionalDirectTaken
		<< ", " << PeriodicNumBranches[i].UnconditionalIndirectTaken << '\n';

	//Reset the variables
	PeriodicNumBranches[i].Total = PeriodicNumBranches[i].ConditionalDirectTaken = PeriodicNumBranches[i].ConditionalDirectNotTaken = 0;
	PeriodicNumBranches[i].UnconditionalDirectTaken = PeriodicNumBranches[i].UnconditionalIndirectTaken = 0;
}
/* Mounika edit end*/

//open file/pipe
VOID openOutput()
{
	// Writing traces to a respective file depending on input options
	if (TraceIsOn.Value() == TRUE){
		//Check compression knob
		std::string compressor = Compression.Value();
		if (compressor != "0" && compressor != "bzip2" && compressor != "pbzip2" && compressor != "gzip" && compressor != "pigz")
		{
			std::cout << "mcfProfile: That compression program is not supported" << std::endl;
			std::exit(-1);
		}
		if (compressor != "0")
		{
			//const string temp = "/bin/" + compressor;
			if (access(string("/bin/" + compressor).c_str(), X_OK) == -1 && access(string("/usr/bin/" + compressor).c_str(), X_OK) == -1)
			{
				std::cout << "mcfProfile: This system does not support " << compressor << ". Exiting..." << std::endl;
				std::exit(-1);
			}
			usingCompression = TRUE;
		}
		else
			usingCompression = FALSE;

		//attach extension
		std::string tempFileName;
		if (ASCII.Value() == FALSE)
			tempFileName = fileName + ".bin";
		else
			tempFileName = fileName + ".txt";

		if (usingCompression)
		{
			char temp[1000] = { 0 };
			if (compressor == "bzip2")
			{
				tempFileName += ".bz2";
				sprintf(temp, "bzip2 > %s", tempFileName.c_str());
				outPipe = popen(temp, "w");
			}
			else if (compressor == "pbzip2")
			{
				tempFileName += ".bz2";
				sprintf(temp, "pbzip2 > %s", tempFileName.c_str());
				outPipe = popen(temp, "w");
			}
			else if (compressor == "gzip")
			{
				tempFileName += ".gz";
				sprintf(temp, "gzip > %s", tempFileName.c_str());
				outPipe = popen(temp, "w");
			}
			else
			{
				tempFileName += ".gz";
				sprintf(temp, "pigz > %s", tempFileName.c_str());
				outPipe = popen(temp, "w");
			}

			if (outPipe == NULL)
			{
				std::cout << "mcfProfile: Error opening outPipe..." << std::endl;
				std::exit(-1);
			}
		}
		//if not using compression, write to raw file
		else
		{
			if (ASCII.Value() == FALSE)
				OutFile.open(tempFileName.c_str(), ios::out | ios::binary);
			else
				OutFile.open(tempFileName.c_str());
		}

		statFileName = tempFileName;

		THREADID err;
		//Spawn write thread
		if (ASCII.Value())
			err = PIN_SpawnInternalThread(ThreadWriteASCII, NULL, 0, NULL);
		else
			err = PIN_SpawnInternalThread(ThreadWriteBin, NULL, 0, NULL);

		if (err == INVALID_THREADID)
			std::cout << "Error creating internal Pin thread. Exiting.." << std::endl;
	}
	// file to print final statistics
	string nameOfFile = fileName + ".Statistics";
	StatFile.open(nameOfFile.c_str());

	// create a file to write dynamic characteristics for each thread
	if (Dynamic.Value()){
		DynamicStatFile = new std::ofstream[appThreads.Value()];
		for (UINT8 i = 0; i < appThreads.Value(); i++){
			std::ostringstream nameOfFile;
			nameOfFile << fileName << "_" << (UINT16)i << "_Dynamic.Statistics";
			DynamicStatFile[i].open(nameOfFile.str().c_str());
			DynamicStatFile[i] << "threadid, elapsed instructions, Control Transfer: total, Conditional Direct Taken, Conditional Direct Not Taken,  Unconditional Direct, Unconditional Indirect" << '\n';
		}
	}
}
VOID closeOutput()
{
   if( usingCompression ) 
      pclose(outPipe);
   else
      OutFile.close();

   ASCIIDescriptorTable.clear();
   binDescriptorTable.clear();
   if (IsEndOfApplication){
	   if (Dynamic.Value()){
		   for (int i = 0; i < numThreads; i++)
			   DynamicStatFile[i].close();
	   }
	   StatFile.close();
   }
}
