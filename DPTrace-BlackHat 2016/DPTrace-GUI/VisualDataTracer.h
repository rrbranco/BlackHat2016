#ifndef __VISUAL_DATA_TRACER_H__
#define __VISUAL_DATA_TRACER_H__

#include <vcclr.h>
#include <limits.h>

using namespace System;
using namespace System::Collections;
using namespace System::Web::UI;

#define VDT_FILE_MAGIC "VDT"
#define VDT_ENTRY_SIZE 256

/*
 * This class represents an entry in the analysis/trace file.
 * The first 5 members refer to the basic info needed for the
 * analyzer to work, and the last one is a full representation of
 * the instruction, stored for pretty output to the user.
 */

ref class VdtInstruction
{
public:
	String ^Mnem;
	String ^Dst;
	String ^Src;
	String ^SrcDep1;
	String ^SrcDep2;
	String ^SrcDep3;
	String ^FullDump;

	/*
	 * The constructor. Receives a 256-bytes buffer read from the
	 * analysis file and cuts/paste everything into their own place.
	 */

	VdtInstruction(char *unsignedRaw)
	{
		Mnem = gcnew String(unsignedRaw);
		Dst = gcnew String(unsignedRaw + 8);
		Src = gcnew String(unsignedRaw + 24);
		SrcDep1 = gcnew String(unsignedRaw + 40);
		SrcDep2 = gcnew String(unsignedRaw + 44);
		SrcDep3 = gcnew String(unsignedRaw + 48);
		FullDump = gcnew String(unsignedRaw + 52);
	}
};

/*
 * This is the queue structure used in the BFS search.
 * Pairs up a target whose definition we're looking for (can be
 * a register or memory address preceded by *) and an index
 * into the trace list box. The index is used to state 'start
 * looking for this target's definition from this point and
 * backwards' (remember this is backwards dataflow analysis).
 */
ref class VdtTarget
{
public:
	String 				^TargetName;
	unsigned long long	InstrIndx;
	Pair				^OutChain;

	VdtTarget(String ^tName, unsigned long long iIndx, Pair ^oChain)
	{
		TargetName = tName;
		InstrIndx = iIndx;
		OutChain = oChain;
	}
};

/*
 * This is a simple mapping between the different names under
 * which a same memory area (a register, in this case) can be
 * addressed. More specifically, if we're looking for, say, a 
 * instruction that wrote to 'eax', it's OK to return one that
 * writes to 'ah', for 'ah' is part of 'eax'.
 */
static char *EAX[] = {"al", "ah", "ax", "eax", "rax", ""};
static char *EBX[] = {"bl", "bh", "bx", "ebx", "rbx", ""};
static char *ECX[] = {"cl", "ch", "cx", "ecx", "rcx", ""};
static char *EDX[] = {"dl", "dh", "dx", "edx", "rdx", ""};
static char *ESI[] = {"si", "esi", "rsi", ""};
static char *EDI[] = {"di", "edi", "rdi", ""};
static char *EBP[] = {"bp", "ebp", "rbp", ""};
static char *ESP[] = {"sp", "esp", "rsp", ""};



ref class VDTAnalyzer
{
	/*
	 * TODO: perhaps implement a 'no-user-input' mode. I.e. search until
	 * the end and output which memory values the user would need to control
	 * to influence the target memory. Obs.: this perhaps requires considering
	 * in/out instructions and taking them as end-points.
	 */
public: 
	/*
	 * Map to-from 'equivalent' register names, as previously discussed.
	 * Maps a string, such as 'eax', to a string array, such as the 'EAX'
	 * defined above.
	 */
	static Hashtable ^GlobalRegisterMap;
	// Auxiliary queue used for the analysis algorithm
	static Queue ^GlobalTargetQueue;
	/*
	 * This is a search-optimizing structure, designed to keep us from
	 * naively repeating searches.
	 * It's a set of entries that look like 00bacada:ebx, meaning that
	 * we already started a search for a definition of 'ebx' starting
	 * from the file offset 0xbacada and, therefore, shouldn't bother
	 * queueing this search request again. We otherwise would have chances
	 * to find ourselves in such a mischieving situation because we
	 * do 'forked' searches, e.g.:
	 *
	 *  ...
	 *  1) mov eax, ebx
	 *  2) mov [eax], eax
	 *  3) mov ecx, [eax]
	 *  ...
	 *
	 * Suppose we're searching for ecx's definition and eventually find it
	 * at instruction number '3'. At this moment, this one line of search
	 * forks into two: one is interested in finding eax's definition, whereas
	 * the other wants to know when the memory pointed by eax (say, 0xdeadf0f0)
	 * is written to. The first line of search goes straight to instruction '1'.
	 * The second one visits instruction '2' but then goes on and eventually
	 * also finds itself at instruction '1'. At this moment, both lines of
	 * search are willing to proceed exactly like one another (i.e. searching
	 * for ebx's definition). The introduction of the 'visited' set, however,
	 * will allow only one of them to carry on, effectively 'merging' them like
	 * it should.
	 */
	static ArrayList ^GlobalVisitedSet;

	// Taint ranges (key = start address; value = (unsigned long)  End address)
	static Hashtable ^GlobalTaintRanges;
	// Instruction definitions (key = instruction index; value = a VdtInstruction)
	static Hashtable ^GlobalInstrMap;

	static ArrayList ^SearchTaintOf(String ^target, unsigned long long startingIndx);
	static void InitRegMap();

private:
	static bool MatchRegs(String ^reg1, String ^reg2);
	static unsigned long long SearchDef(unsigned long long startIndx, String ^target);
	static void PushReg(String ^reg, unsigned long long defPos, VdtTarget ^curTarget);
};

#endif