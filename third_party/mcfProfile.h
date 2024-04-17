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

//Bugs and criticisms can be directed to
//myers.albert@gmail.com

//mcfProfile.h
//contains instruction analysis functions injected with mcfProfile.cpp

/*--------------------------*/
/* ASCII analysis routines  */ 
/*--------------------------*/

//Unconditional Direct Branches
VOID Emit_UnconditionalDirect_ASCII(const THREADID threadid, const ADDRINT address, const ADDRINT target)
{ 
   if( !CanEmit(threadid) ) return;
   /* Mounika edit start*/
   // get the logically ordered id corresponding to original threadid
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalDirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //Always taken 
   std::string type = ", U, D, T"; 
   //Descriptor String
   std::ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target
           << type << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
    PIN_GetLock(&table_lock, threadid+1);
   //Push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}
//Conditional Direct Branches
VOID Emit_ConditionalDirect_ASCII(const THREADID threadid, const ADDRINT address, const ADDRINT target, const BOOL taken)
{
   if( !CanEmit(threadid) ) return;
   /* Mounika edit start*/
   // get the logically ordered id corresponding to original threadid
   UINT8 assignedTID = get_localtid(threadid);
   std::string type;
   //Direct conditional, check if taken
   if(taken == 0)
   {
	   IncrementEachThreadBranchStatistics(ConditionalDirectNotTaken, assignedTID);
      type = ", C, D, NT";
   }
   else
   {
	   IncrementEachThreadBranchStatistics(ConditionalDirectTaken, assignedTID);
      type = ", C, D, T";
   }
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   std::ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target 
           << type << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
    PIN_GetLock(&table_lock, threadid+1);
   //Push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}
//Unconditional Direct Branches
VOID Emit_UnconditionalIndirect_ASCII(const THREADID threadid, const ADDRINT address, const ADDRINT target)
{
   if( !CanEmit(threadid) ) return;
   /* Mounika edit start*/
   // get the logically ordered id corresponding to original threadid
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalIndirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   std::string type = ", U, I, T"; 
   std::ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target 
           << type << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
    PIN_GetLock(&table_lock, threadid+1);
   //Push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}
/*--------------------------*/
/* ASCII with Disassembly analysis routines  */ 
/*--------------------------*/

//Unconditional Direct Branches
VOID Emit_UnconditionalDirect_ASCII_Dis(const THREADID threadid, const ADDRINT address, const ADDRINT target)
{ 
   if( !CanEmit(threadid) ) return;
   /* Mounika edit start*/
   // get the logically ordered id corresponding to original threadid
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalDirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //Disassembly
   std::string ins;
   getAssemblyString(ins, threadid, address);
 
   //Always taken 
   std::string type = ", U, D, T"; 
   //Descriptor String
   ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target 
           << type 
           << "\t" << ins << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
    PIN_GetLock(&table_lock, threadid+1);
   //Push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}

//Conditional Direct Branches
VOID Emit_ConditionalDirect_ASCII_Dis(const THREADID threadid, const ADDRINT address, const ADDRINT target, const BOOL taken)
{
   if( !CanEmit(threadid) ) return;
   /* Mounika edit start*/
   // get the logically ordered id corresponding to original threadid
   UINT8 assignedTID = get_localtid(threadid);
   //Disassembly
   std::string ins;
   getAssemblyString(ins, threadid, address);

   //Direct conditional so check if taken
   std::string type;
   if(taken == 0)
   {
      type = ", C, D, NT";
	  IncrementEachThreadBranchStatistics(ConditionalDirectNotTaken, assignedTID);
   }
   else
   {
	   IncrementEachThreadBranchStatistics(ConditionalDirectTaken, assignedTID);
      type = ", C, D, T";
   }
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   std::ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target 
           << type 
           << "\t" << ins << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
    PIN_GetLock(&table_lock, threadid+1);
   //Push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}
//Unconditional Indirect Branches
VOID Emit_UnconditionalIndirect_ASCII_Dis(const THREADID threadid, const ADDRINT address, const ADDRINT target)
{
   if( !CanEmit(threadid) ) return;
   //get local id
   /* Mounika edit start*/
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalIndirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //Disassembly
   std::string ins;
   getAssemblyString(ins, threadid, address);
   
   std::string type = ", U, I, T"; 
   std::ostringstream convert;
   convert << std::dec << static_cast<UINT32>(assignedTID)
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << address 
           << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << target
           << type << "\t" << ins << std::endl;
   std::string ASCIIDescriptor = convert.str();
 
   //critical section
    PIN_GetLock(&table_lock, threadid+1);
   //push back, will write on close
   ASCIIDescriptorTable.push_back(ASCIIDescriptor);
   //increment file counter
   IncrementFileCount(ASCIIDescriptor.size());
    PIN_ReleaseLock(&table_lock);
}

/*--------------------------*/
/* Binary analysis routines  */
/*--------------------------*/

//Unconditional Direct Branches
VOID Emit_UnconditionalDirect_Bin(const THREADID threadid,const ADDRINT address,const ADDRINT target)
{
   if( !CanEmit(threadid) ) return;
   //get local id
   /* Mounika edit start*/
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalDirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //setup descriptor
   BinaryDescriptorTableEntry binDescriptor;
   binDescriptor.tid = assignedTID;
   binDescriptor.branchAddress = address;
   binDescriptor.targetAddress = target;
   binDescriptor.branchType = UnconditionalDirectTaken;

   //critical section
    PIN_GetLock(&table_lock, threadid+1);
   binDescriptorTable.push_back(binDescriptor);
   //increment file counter
   IncrementFileCount(BinDescriptorTableEntrySize);
    PIN_ReleaseLock(&table_lock);
}
VOID Emit_ConditionalDirect_Bin(const THREADID threadid,const ADDRINT address,const ADDRINT target,const BOOL taken)
{
   if( !CanEmit(threadid) ) return;
   //get local id
   /* Mounika edit start*/
   UINT8 assignedTID = get_localtid(threadid);
   //setup descriptor
   BinaryDescriptorTableEntry binDescriptor;
   binDescriptor.tid = assignedTID;
   binDescriptor.branchAddress = address;
   binDescriptor.targetAddress = target;
   //If taken paramater will be non-zero 
   if(taken == 0)
   {
	  IncrementEachThreadBranchStatistics(ConditionalDirectNotTaken, assignedTID);
      binDescriptor.branchType = ConditionalDirectNotTaken;
   }
   else
   {
	  IncrementEachThreadBranchStatistics(ConditionalDirectTaken, assignedTID);
      binDescriptor.branchType = ConditionalDirectTaken;
   }
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //critical section
    PIN_GetLock(&table_lock, threadid+1);
   binDescriptorTable.push_back(binDescriptor);
   //increment file counter
   IncrementFileCount(BinDescriptorTableEntrySize);
    PIN_ReleaseLock(&table_lock);
}

VOID Emit_UnconditionalIndirect_Bin(const THREADID threadid,const ADDRINT address,const ADDRINT target)
{
   if( !CanEmit(threadid) ) return;
   //get local id
   /* Mounika edit start*/
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalIndirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //setup descriptor  
   BinaryDescriptorTableEntry binDescriptor;
   binDescriptor.tid = assignedTID;
   binDescriptor.branchAddress = address;
   binDescriptor.targetAddress = target;
   binDescriptor.branchType = UnconditionalIndirectTaken;
   
   //critical section
    PIN_GetLock(&table_lock, threadid+1);
   binDescriptorTable.push_back(binDescriptor);
   //increment file counter
   IncrementFileCount(BinDescriptorTableEntrySize);
    PIN_ReleaseLock(&table_lock);
}

/*--------------------------*/
/* Other analysis routines  */
/*--------------------------*/

//used to decrement fastfoward_count and trace length for all instructions
VOID SetFastForwardAndLength(const THREADID threadid)
{
   //critical section
    PIN_GetLock(&count_lock, threadid+1);
   //if file limit is reacher or trace limit is reached
   //exit tool
   if( (traceLength + 1) == Length.Value() && !noLength )
   {  
      PIN_Detach();
      PIN_ReleaseLock(&count_lock);
      return;
   } 
   //decrement fast forward count   
   if (fastforwardLength < Skip.Value()){
	   fastforwardLength++;
	   PIN_ReleaseLock(&count_lock);
   }
   //otherwise increment length
   else{
	   traceLength++;
	   PIN_ReleaseLock(&count_lock);
	   int id = get_localtid(threadid);
	   //check whether it is time to dump the statisctics
	   if ((traceLengthInCurrentPeriod[id] == PeriodOfIns.Value()) && (Dynamic.Value() == TRUE)){
		   PrintDynamicStatistics(id, traceLengthInCurrentPeriod[id]);
		   //reset the counter
		   traceLengthInCurrentPeriod[id] = 0;
	   }
	   traceLengthInCurrentPeriod[id]++;
   }

   return;
}

//called on signals
VOID Sig(THREADID threadid, CONTEXT_CHANGE_REASON reason, const CONTEXT *ctxtFrom, CONTEXT *ctxtTo, INT32 sig, VOID *v)
{
   if( !CanEmit(threadid) ) return;
   //get local id
   /* Mounika edit start*/
   UINT8 assignedTID = get_localtid(threadid);
   IncrementEachThreadBranchStatistics(UnconditionalIndirectTaken, assignedTID);
   //If tracing is not turned on no need to proceed further
   if (TraceIsOn.Value() == FALSE) return;
   /* Mounika edit end*/

   //From the Pin user guide
   //1. If the tool acquires any locks from within a Pin call-back, it must release those locks before returning from that call-back. 
   //Holding a lock across Pin call-backs violates the hierarchy with respect to the Pin internal locks.
   //
   //2. If the tool calls a Pin API from within a Pin call-back or analysis routine, it should not hold any tool locks when calling the 
   //API. Some of the Pin APIs use the internal Pin locks so holding a tool lock before invoking these APIs violates the hierarchy with 
   //respect to the Pin internal locks.    
    PIN_GetLock(&cout_lock, threadid+1);

   std::cout << "mcfProfile: Thread " << static_cast<UINT16>(assignedTID)  << " received signal " << sig << std::endl;
   if(reason == CONTEXT_CHANGE_REASON_FATALSIGNAL)
   {
      std::cout << "mcfProfile: Fatal signal, exiting immediately..." << std::endl;
       PIN_GetLock(&table_lock, threadid+1);
      closeOutput();
       PIN_ReleaseLock(&table_lock);
      return;
   }
    PIN_ReleaseLock(&cout_lock);

   //get context
   ADDRINT from = PIN_GetContextReg(ctxtFrom, REG_INST_PTR);
   ADDRINT to = PIN_GetContextReg(ctxtTo, REG_INST_PTR);

   //Exception handling and returns from exception handlers is an indirect branch 
   if( ASCII.Value() )
   {
       
      std::ostringstream convert;
      convert << std::dec << static_cast<UINT16>(assignedTID)
                     << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << from 
                     << ", 0x" << std::hex << std::setw(ADDRWID) << std::setfill('0') << to 
                     << std::dec << ", U, I, T";

      if( AnnotateDisassembly.Value() )
      {
         std::string ins;
         getAssemblyString(ins, threadid, from);

         if(reason==CONTEXT_CHANGE_REASON_SIGNAL)
               ins = "\tSignal At: " + ins;
         else if(reason==CONTEXT_CHANGE_REASON_SIGRETURN)
               ins = "\tReturning from signal handler: " + ins;
         convert << ins;
      }
      convert << std::endl;
      std::string ASCIIDescriptor = convert.str();

       PIN_GetLock(&table_lock, threadid+1);
      ASCIIDescriptorTable.push_back(ASCIIDescriptor);
      //increment file counter
      IncrementFileCount(ASCIIDescriptor.size());
       PIN_ReleaseLock(&table_lock);    
   }
   else
   {
      BinaryDescriptorTableEntry binDescriptor;
      binDescriptor.tid = assignedTID;
      binDescriptor.branchAddress = from;
      binDescriptor.targetAddress = to;
      binDescriptor.branchType = UnconditionalIndirectTaken;

      //critical section
       PIN_GetLock(&table_lock, threadid+1);
      binDescriptorTable.push_back(binDescriptor);       
      //increment file counter
      IncrementFileCount(BinDescriptorTableEntrySize);
       PIN_ReleaseLock(&table_lock);
   }
}
