/*++

 Visual Data Tracer v1.0 Alpha
 Copyright (C) 2008-2010  
	Rodrigo Rubira Branco (BSDaemon) <rodrigo@risesecurity.org>
	Julio Auto <julio@julioauto.com>

 Header file for the VDT tracer. This contains the structures
 and the instruction information base used by the project.

 Notes:
    * An empty (NULL) operand means there's no such operand (dst or src).
	* An hyphen ("-") means the operand must be fetched from the disassembly.
	* An x ("x") in the src operand means it's the same as the dst operand.
	* An asterisk ("*") in the src operand means the combination of "-" and "x".
    * The 'register' EDA means EDX:EAX.

 Module Name:

    vdt-tracer.h

--*/

#include <windows.h>

//
// Define KDEXT_64BIT to make all wdbgexts APIs recognize 64 bit addresses
// It is recommended for extensions to use 64 bit headers from wdbgexts so
// the extensions could support 64 bit targets.
//
#define KDEXT_64BIT
#include <wdbgexts.h>
#include <DbgEng.h>

#define VDT_FILE_MAGIC "VDT"


typedef struct VdtInstr_t{
	char *Dst;
	char *Src;

	VdtInstr_t ()
	{
		Dst = NULL;
		Src = NULL;
	}
	VdtInstr_t (char *newDst, char *newSrc)
	{
		Dst = newDst;
		Src = newSrc;
	}
} VdtInstr;

struct compare
{
	bool operator() (CHAR *lhs, CHAR* rhs) const
	{
		return (strcmp(lhs, rhs) < 0);
	}
};

map<CHAR *, VdtInstr, compare> InstructionDefs;

static void vdt_initmap()
{
	InstructionDefs["00-add"] = VdtInstr_t("-", "-");
    InstructionDefs["01-add"] = VdtInstr_t("-", "-");
    InstructionDefs["02-add"] = VdtInstr_t("-", "-");
    InstructionDefs["03-add"] = VdtInstr_t("-", "-");
    InstructionDefs["04-add"] = VdtInstr_t("-", "-");
    InstructionDefs["05-add"] = VdtInstr_t("-", "-");
    InstructionDefs["06-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["07-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["08-or"] = VdtInstr_t("-", "-");
    InstructionDefs["09-or"] = VdtInstr_t("-", "-");
    InstructionDefs["0a-or"] = VdtInstr_t("-", "-");
    InstructionDefs["0b-or"] = VdtInstr_t("-", "-");
    InstructionDefs["0c-or"] = VdtInstr_t("-", "-");
    InstructionDefs["0d-or"] = VdtInstr_t("-", "-");
    InstructionDefs["0e-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["10-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["11-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["12-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["13-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["14-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["15-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["16-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["17-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["18-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["19-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["1a-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["1b-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["1c-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["1d-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["1e-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["1f-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["20-and"] = VdtInstr_t("-", "-");
    InstructionDefs["21-and"] = VdtInstr_t("-", "-");
    InstructionDefs["22-and"] = VdtInstr_t("-", "-");
    InstructionDefs["23-and"] = VdtInstr_t("-", "-");
    InstructionDefs["24-and"] = VdtInstr_t("-", "-");
    InstructionDefs["25-and"] = VdtInstr_t("-", "-");
    InstructionDefs["27-daa"] = VdtInstr_t("al", NULL);
    InstructionDefs["28-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["29-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["2a-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["2b-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["2c-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["2d-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["2f-das"] = VdtInstr_t("al", NULL);
    InstructionDefs["30-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["31-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["32-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["33-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["34-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["35-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["37-aaa"] = VdtInstr_t("al", NULL);
    InstructionDefs["38-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["39-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["3a-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["3b-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["3c-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["3d-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["3f-aas"] = VdtInstr_t("al", NULL);
    InstructionDefs["40-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["41-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["42-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["43-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["44-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["45-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["46-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["47-inc"] = VdtInstr_t("-", "1");
    InstructionDefs["48-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["49-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4a-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4b-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4c-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4d-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4e-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["4f-dec"] = VdtInstr_t("-", "1");
    InstructionDefs["50-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["58-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["60-pusha"] = VdtInstr_t(NULL, "ax");
    InstructionDefs["60-pushad"] = VdtInstr_t(NULL, "eax");
    InstructionDefs["61-popa"] = VdtInstr_t("di", NULL);
    InstructionDefs["61-popad"] = VdtInstr_t("edi", NULL);
    InstructionDefs["62-bound"] = VdtInstr_t(NULL, "-");
    InstructionDefs["63-arpl"] = VdtInstr_t(NULL, "-");
    InstructionDefs["63-movsxd"] = VdtInstr_t("-", "-");
    InstructionDefs["68-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["69-imul"] = VdtInstr_t("-", "-");
    InstructionDefs["6a-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["6b-imul"] = VdtInstr_t("-", "x");
    InstructionDefs["6c-ins"] = VdtInstr_t("-", NULL);
    InstructionDefs["6c-insb"] = VdtInstr_t("-", NULL);
    InstructionDefs["6d-ins"] = VdtInstr_t("-", NULL);
    InstructionDefs["6d-insw"] = VdtInstr_t("-", NULL);
    InstructionDefs["6d-insd"] = VdtInstr_t("-", NULL);
    InstructionDefs["6e-outs"] = VdtInstr_t(NULL, "-");
    InstructionDefs["6e-outsb"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["6f-outs"] = VdtInstr_t(NULL, "-");
    InstructionDefs["6f-outsw"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["6f-outsd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["70-jo"] = VdtInstr_t(NULL, "-");
    InstructionDefs["71-jno"] = VdtInstr_t(NULL, "-");
    InstructionDefs["72-jb"] = VdtInstr_t(NULL, "-");
    InstructionDefs["72-jnae"] = VdtInstr_t(NULL, "-");
    InstructionDefs["72-jc"] = VdtInstr_t(NULL, "-");
    InstructionDefs["73-jnb"] = VdtInstr_t(NULL, "-");
    InstructionDefs["73-jae"] = VdtInstr_t(NULL, "-");
    InstructionDefs["73-jnc"] = VdtInstr_t(NULL, "-");
    InstructionDefs["74-jz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["74-je"] = VdtInstr_t(NULL, "-");
    InstructionDefs["75-jnz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["75-jne"] = VdtInstr_t(NULL, "-");
    InstructionDefs["76-jbe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["76-jna"] = VdtInstr_t(NULL, "-");
    InstructionDefs["77-jnbe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["77-ja"] = VdtInstr_t(NULL, "-");
    InstructionDefs["78-js"] = VdtInstr_t(NULL, "-");
    InstructionDefs["79-jns"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7a-jp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7a-jpe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7b-jnp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7b-jpo"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7c-jl"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7c-jnge"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7d-jnl"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7d-jge"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7e-jle"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7e-jng"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7f-jnle"] = VdtInstr_t(NULL, "-");
    InstructionDefs["7f-jg"] = VdtInstr_t(NULL, "-");
    InstructionDefs["80-add"] = VdtInstr_t("-", "-");
    InstructionDefs["80-or"] = VdtInstr_t("-", "-");
    InstructionDefs["80-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["80-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["80-and"] = VdtInstr_t("-", "-");
    InstructionDefs["80-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["80-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["80-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["81-add"] = VdtInstr_t("-", "-");
    InstructionDefs["81-or"] = VdtInstr_t("-", "-");
    InstructionDefs["81-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["81-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["81-and"] = VdtInstr_t("-", "-");
    InstructionDefs["81-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["81-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["81-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["82-add"] = VdtInstr_t("-", "-");
    InstructionDefs["82-or"] = VdtInstr_t("-", "-");
    InstructionDefs["82-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["82-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["82-and"] = VdtInstr_t("-", "-");
    InstructionDefs["82-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["82-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["82-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["83-add"] = VdtInstr_t("-", "-");
    InstructionDefs["83-or"] = VdtInstr_t("-", "-");
    InstructionDefs["83-adc"] = VdtInstr_t("-", "-");
    InstructionDefs["83-sbb"] = VdtInstr_t("-", "-");
    InstructionDefs["83-and"] = VdtInstr_t("-", "-");
    InstructionDefs["83-sub"] = VdtInstr_t("-", "-");
    InstructionDefs["83-xor"] = VdtInstr_t("-", "-");
    InstructionDefs["83-cmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["84-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["85-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["86-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["87-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["88-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["89-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["8a-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["8b-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["8c-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["8d-lea"] = VdtInstr_t("-", "-");
    InstructionDefs["8e-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["8f-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["90-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["90-nop"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["90-pause"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["91-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["92-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["93-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["94-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["95-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["96-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["97-xchg"] = VdtInstr_t("-", NULL);
    InstructionDefs["98-cbw"] = VdtInstr_t("ax", "al");
    InstructionDefs["98-cwde"] = VdtInstr_t("eax", "ax");
    InstructionDefs["98-cdqe"] = VdtInstr_t("rax", "eax");
    InstructionDefs["99-cwd"] = VdtInstr_t("da", "ax");
    InstructionDefs["99-cdq"] = VdtInstr_t("eda", "eax");
    InstructionDefs["99-cqo"] = VdtInstr_t("rda", "rax");
    InstructionDefs["9a-callf"] = VdtInstr_t(NULL, "-");
    InstructionDefs["9c-pushf"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9c-pushfd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9c-pushfq"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9d-popf"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9d-popfd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9d-popfq"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["9e-sahf"] = VdtInstr_t(NULL, "ah");
    InstructionDefs["9f-lahf"] = VdtInstr_t("ah", NULL);
    InstructionDefs["a0-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["a1-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["a2-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["a3-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["a4-movs"] = VdtInstr_t("-", "-");
    InstructionDefs["a4-movsb"] = VdtInstr_t("-", "-");
    InstructionDefs["a5-movs"] = VdtInstr_t("-", "-");
    InstructionDefs["a5-movsw"] = VdtInstr_t("-", "-");
    InstructionDefs["a5-movsd"] = VdtInstr_t("-", "-");
    InstructionDefs["a5-movsq"] = VdtInstr_t("-", "-");
    InstructionDefs["a6-cmps"] = VdtInstr_t(NULL, "-");
    InstructionDefs["a6-cmpsb"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["a7-cmps"] = VdtInstr_t(NULL, "-");
    InstructionDefs["a7-cmpsw"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["a7-cmpsd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["a7-cmpsq"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["a8-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["a9-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["aa-stos"] = VdtInstr_t("-", "al");
    InstructionDefs["aa-stosb"] = VdtInstr_t("-", "al");
    InstructionDefs["ab-stos"] = VdtInstr_t("-", "eax");
    InstructionDefs["ab-stosw"] = VdtInstr_t("-", "ax");
    InstructionDefs["ab-stosd"] = VdtInstr_t("-", "eax");
    InstructionDefs["ab-stosq"] = VdtInstr_t("-", "rax");
    InstructionDefs["ac-lods"] = VdtInstr_t("al", "-");
    InstructionDefs["ac-lodsb"] = VdtInstr_t("al", "-");
    InstructionDefs["ad-lodsw"] = VdtInstr_t("ax", "-");
    InstructionDefs["ad-lods"] = VdtInstr_t("eax", "-");
    InstructionDefs["ad-lodsd"] = VdtInstr_t("eax", "-");
    InstructionDefs["ad-lodsq"] = VdtInstr_t("rax", "-");
    InstructionDefs["ae-scas"] = VdtInstr_t(NULL, "-");
    InstructionDefs["ae-scasb"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["af-scas"] = VdtInstr_t(NULL, "-");
    InstructionDefs["af-scasw"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["af-scasd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["af-scasq"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["b0-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b1-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b2-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b3-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b4-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b5-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b6-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b7-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b8-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["b9-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["ba-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["bb-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["bc-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["bd-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["be-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["bf-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["c0-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["c1-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["c2-ret"] = VdtInstr_t(NULL, "-");
    InstructionDefs["c3-ret"] = VdtInstr_t(NULL, NULL);

	// We need some extra analysis here, to be really accurate
    InstructionDefs["c4-les"] = VdtInstr_t("-", "-");
    InstructionDefs["c5-lds"] = VdtInstr_t("-", "-");

    InstructionDefs["c6-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["c7-mov"] = VdtInstr_t("-", "-");

	/* We ignore these because they're not useful

    InstructionDefs["c8-enter"] = VdtInstr_t("rbp", "-");
    InstructionDefs["c9-leave"] = VdtInstr_t("rbp", NULL);

	*/
    InstructionDefs["ca-ret"] = VdtInstr_t(NULL, "-");
    InstructionDefs["cb-ret"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["cc-int"] = VdtInstr_t(NULL, "-");
    InstructionDefs["cd-int"] = VdtInstr_t(NULL, "-");
    InstructionDefs["ce-into"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["cf-iret"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["cf-iretd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["cf-iretq"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["d0-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["d0-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["d1-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["d2-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-rol"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-ror"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-rcl"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-rcr"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-shr"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-sal"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-shl"] = VdtInstr_t("-", "-");
    InstructionDefs["d3-sar"] = VdtInstr_t("-", "-");
    InstructionDefs["d4-aam"] = VdtInstr_t("al", NULL);
	
	// This things doesn't really exist - we're not ready for it
    // InstructionDefs["d4-amx"] = VdtInstr_t("al", "-");
    
	InstructionDefs["d5-aad"] = VdtInstr_t("al", NULL);
	
	// This things doesn't really exist - we're not ready for it
    // InstructionDefs["d5-adx"] = VdtInstr_t("al", "-");
    
	InstructionDefs["d6-salc"] = VdtInstr_t("al", NULL);
    InstructionDefs["d6-setalc"] = VdtInstr_t("al", NULL);
    InstructionDefs["d7-xlat"] = VdtInstr_t("al", "-");
    InstructionDefs["d7-xlatb"] = VdtInstr_t("al", NULL);
    InstructionDefs["e0-loopnz"] = VdtInstr_t("rcx", "-");
    InstructionDefs["e0-loopne"] = VdtInstr_t("rcx", "-");
    InstructionDefs["e1-loopz"] = VdtInstr_t("rcx", "-");
    InstructionDefs["e1-loope"] = VdtInstr_t("rcx", "-");
    InstructionDefs["e2-loop"] = VdtInstr_t("rcx", "-");
    InstructionDefs["e3-jcxz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e3-jecxz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e3-jrcxz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e4-in"] = VdtInstr_t("-", "-");
    InstructionDefs["e5-in"] = VdtInstr_t("-", "-");
    InstructionDefs["e6-out"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e7-out"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e8-call"] = VdtInstr_t(NULL, "-");
    InstructionDefs["e9-jmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["ea-jmpf"] = VdtInstr_t(NULL, "-");
    InstructionDefs["eb-jmp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["ec-in"] = VdtInstr_t("-", "-");
    InstructionDefs["ed-in"] = VdtInstr_t("-", "-");
    InstructionDefs["ee-out"] = VdtInstr_t(NULL, "-");
    InstructionDefs["ef-out"] = VdtInstr_t(NULL, "-");
    InstructionDefs["f1-int1"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["f1-icebp"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["f4-hlt"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["f5-cmc"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["f6-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["f6-not"] = VdtInstr_t("-", NULL);
    InstructionDefs["f6-neg"] = VdtInstr_t("-", NULL);
    InstructionDefs["f6-mul"] = VdtInstr_t("ax", "*"); // Really needs special parsing :( - the '*' here isn't completely true
    InstructionDefs["f6-imul"] = VdtInstr_t("ax", "*"); // Really needs special parsing :( - the '*' here isn't completely true
    InstructionDefs["f6-div"] = VdtInstr_t("ax", "*");
    InstructionDefs["f6-idiv"] = VdtInstr_t("ax", "*");
    InstructionDefs["f7-test"] = VdtInstr_t(NULL, "-");
    InstructionDefs["f7-not"] = VdtInstr_t("-", NULL);
    InstructionDefs["f7-neg"] = VdtInstr_t("-", NULL);
    InstructionDefs["f7-mul"] = VdtInstr_t("eda", "*"); // Really needs special parsing :( - the '*' here isn't completely true
    InstructionDefs["f7-imul"] = VdtInstr_t("eda", "*"); // Really needs special parsing :( - the '*' here isn't completely true
    InstructionDefs["f7-div"] = VdtInstr_t("eda", "*");
    InstructionDefs["f7-idiv"] = VdtInstr_t("eda", "*");
    InstructionDefs["f8-clc"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["f9-stc"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["fa-cli"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["fb-sti"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["fc-cld"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["fd-std"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["fe-inc"] = VdtInstr_t("-", NULL);
    InstructionDefs["fe-dec"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-inc"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-dec"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-call"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-callf"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-jmp"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-jmpf"] = VdtInstr_t("-", NULL);
    InstructionDefs["ff-push"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f00-sldt"] = VdtInstr_t("-", "ldtr");
    InstructionDefs["0f00-str"] = VdtInstr_t("-", "tr");
    InstructionDefs["0f00-lldt"] = VdtInstr_t("ldtr", "-");
    InstructionDefs["0f00-ltr"] = VdtInstr_t("tr", "-");
    InstructionDefs["0f00-verr"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f00-verw"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f01-sgdt"] = VdtInstr_t("-", "gdtr");
    InstructionDefs["0f01-sidt"] = VdtInstr_t("-", "idtr");
    InstructionDefs["0f01-lgdt"] = VdtInstr_t("gdtr", "-");
    InstructionDefs["0f01-lidt"] = VdtInstr_t("idtr", "-");
    InstructionDefs["0f01-smsw"] = VdtInstr_t("-", "msw");
    InstructionDefs["0f01-lmsw"] = VdtInstr_t("msw", "-");
    InstructionDefs["0f01-invlpg"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f01-swapgs"] = VdtInstr_t("gs", NULL);
    InstructionDefs["0f02-lar"] = VdtInstr_t("-", "-");
    InstructionDefs["0f03-lsl"] = VdtInstr_t("-", "-");

	// This things doesn't really exist - we're not ready for it
    // InstructionDefs["0f05-loadall"] = VdtInstr_t("ax", NULL);

	// We're not working with ring-0 as of yet, should be fixed later
    // InstructionDefs["0f05-syscall"] = VdtInstr_t("rcx", NULL);

	// This things doesn't really exist - we're not ready for it
    // InstructionDefs["0f07-loadall"] = VdtInstr_t("eax", NULL);

	// We're not working with ring-0 as of yet, should be fixed later
    // InstructionDefs["0f07-sysret"] = VdtInstr_t("ss", "r11");

    InstructionDefs["0f08-invd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["0f09-wbinvd"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["0f0b-ud2"] = VdtInstr_t(NULL, NULL);
    InstructionDefs["0f0d-nop"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f1f-nop"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f20-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["0f21-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["0f22-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["0f23-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["0f24-mov"] = VdtInstr_t("-", "-");
    InstructionDefs["0f26-mov"] = VdtInstr_t("-", "-");
	
	// We're not working with ring-0 as of yet, should be fixed later
    // InstructionDefs["0f30-wrmsr"] = VdtInstr_t("msr", "ecx");
    
	// We log this as a stopping condition for eax and edx (treated in analysis-time)
	InstructionDefs["0f31-rdtsc"] = VdtInstr_t(NULL, NULL);
    
	// We're not working with ring-0 as of yet, should be fixed later
	// InstructionDefs["0f32-rdmsr"] = VdtInstr_t("rax", "rcx");
    
	// We log this as a stopping condition for eax and edx (treated in analysis-time)
	InstructionDefs["0f33-rdpmc"] = VdtInstr_t(NULL, NULL);

	// We're not working with ring-0 as of yet, should be fixed later
    // InstructionDefs["0f34-sysenter"] = VdtInstr_t(NULL, "ia32_sysenter_cs");
	
	// We're not working with ring-0 as of yet, should be fixed later
    // InstructionDefs["0f35-sysexit"] = VdtInstr_t(NULL, "ia32_sysenter_cs");

    InstructionDefs["0f40-cmovo"] = VdtInstr_t("-", "-");
    InstructionDefs["0f41-cmovno"] = VdtInstr_t("-", "-");
    InstructionDefs["0f42-cmovb"] = VdtInstr_t("-", "-");
    InstructionDefs["0f42-cmovnae"] = VdtInstr_t("-", "-");
    InstructionDefs["0f42-cmovc"] = VdtInstr_t("-", "-");
    InstructionDefs["0f43-cmovnb"] = VdtInstr_t("-", "-");
    InstructionDefs["0f43-cmovae"] = VdtInstr_t("-", "-");
    InstructionDefs["0f43-cmovnc"] = VdtInstr_t("-", "-");
    InstructionDefs["0f44-cmovz"] = VdtInstr_t("-", "-");
    InstructionDefs["0f44-cmove"] = VdtInstr_t("-", "-");
    InstructionDefs["0f45-cmovnz"] = VdtInstr_t("-", "-");
    InstructionDefs["0f45-cmovne"] = VdtInstr_t("-", "-");
    InstructionDefs["0f46-cmovbe"] = VdtInstr_t("-", "-");
    InstructionDefs["0f46-cmovna"] = VdtInstr_t("-", "-");
    InstructionDefs["0f47-cmovnbe"] = VdtInstr_t("-", "-");
    InstructionDefs["0f47-cmova"] = VdtInstr_t("-", "-");
    InstructionDefs["0f48-cmovs"] = VdtInstr_t("-", "-");
    InstructionDefs["0f49-cmovns"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4a-cmovp"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4a-cmovpe"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4b-cmovnp"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4b-cmovpo"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4c-cmovl"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4c-cmovnge"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4d-cmovnl"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4d-cmovge"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4e-cmovle"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4e-cmovng"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4f-cmovnle"] = VdtInstr_t("-", "-");
    InstructionDefs["0f4f-cmovg"] = VdtInstr_t("-", "-");
    InstructionDefs["0f80-jo"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f81-jno"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f82-jb"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f82-jnae"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f82-jc"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f83-jnb"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f83-jae"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f83-jnc"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f84-jz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f84-je"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f85-jnz"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f85-jne"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f86-jbe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f86-jna"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f87-jnbe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f87-ja"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f88-js"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f89-jns"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8a-jp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8a-jpe"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8b-jnp"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8b-jpo"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8c-jl"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8c-jnge"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8d-jnl"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8d-jge"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8e-jle"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8e-jng"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8f-jnle"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f8f-jg"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0f90-seto"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f91-setno"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f92-setb"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f92-setnae"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f92-setc"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f93-setnb"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f93-setae"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f93-setnc"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f94-setz"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f94-sete"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f95-setnz"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f95-setne"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f96-setbe"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f96-setna"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f97-setnbe"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f97-seta"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f98-sets"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f99-setns"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9a-setp"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9a-setpe"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9b-setnp"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9b-setpo"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9c-setl"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9c-setnge"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9d-setnl"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9d-setge"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9e-setle"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9e-setng"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9f-setnle"] = VdtInstr_t("-", NULL);
    InstructionDefs["0f9f-setg"] = VdtInstr_t("-", NULL);
    InstructionDefs["0fa0-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0fa1-pop"] = VdtInstr_t("-", NULL);

	// We log this as a stopping condition for eax, ebx, ecx and edx (treated in analysis-time)
    InstructionDefs["0fa2-cpuid"] = VdtInstr_t(NULL, NULL);

    InstructionDefs["0fa3-bt"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0fa4-shld"] = VdtInstr_t("-", "-");
    InstructionDefs["0fa5-shld"] = VdtInstr_t("-", "-");
    InstructionDefs["0fa8-push"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0fa9-pop"] = VdtInstr_t("-", NULL);
    InstructionDefs["0fab-bts"] = VdtInstr_t("-", "-");
    InstructionDefs["0fac-shrd"] = VdtInstr_t("-", "-");
    InstructionDefs["0fad-shrd"] = VdtInstr_t("-", "-");
    InstructionDefs["0faf-imul"] = VdtInstr_t("-", "*");

	// These have been manually written
	// CHANGE THIS SHIT NOW! IMPLEMENT CONDITIONAL TAINTS
    InstructionDefs["0fb0-cmpxchg"] = VdtInstr_t("al", "-");
    InstructionDefs["0fb1-cmpxchg"] = VdtInstr_t("eax", "-"); // Ignoring 16-bit mode here

	// We need some extra analysis here, to be really accurate
    InstructionDefs["0fb2-lss"] = VdtInstr_t("-", "-");

    InstructionDefs["0fb3-btr"] = VdtInstr_t("-", "-");

	// We need some extra analysis here, to be really accurate
    InstructionDefs["0fb4-lfs"] = VdtInstr_t("-", "-");
    InstructionDefs["0fb5-lgs"] = VdtInstr_t("-", "-");

    InstructionDefs["0fb6-movzx"] = VdtInstr_t("-", "-");
    InstructionDefs["0fb7-movzx"] = VdtInstr_t("-", "-");
    InstructionDefs["0fb9-ud"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0fba-bt"] = VdtInstr_t(NULL, "-");
    InstructionDefs["0fba-bts"] = VdtInstr_t("-", "-");
    InstructionDefs["0fba-btr"] = VdtInstr_t("-", "-");
    InstructionDefs["0fba-btc"] = VdtInstr_t("-", "-");
    InstructionDefs["0fbb-btc"] = VdtInstr_t("-", "-");
    InstructionDefs["0fbc-bsf"] = VdtInstr_t("-", "-");
    InstructionDefs["0fbd-bsr"] = VdtInstr_t("-", "-");
    InstructionDefs["0fbe-movsx"] = VdtInstr_t("-", "-");
    InstructionDefs["0fbf-movsx"] = VdtInstr_t("-", "-");
    InstructionDefs["0fc0-xadd"] = VdtInstr_t("-", NULL);
    InstructionDefs["0fc1-xadd"] = VdtInstr_t("-", NULL);

	// These have been manually written
    InstructionDefs["0fc7-cmpxchg8b"] = VdtInstr_t("eda", "-");
    InstructionDefs["0fc7-cmpxchg16b"] = VdtInstr_t("eda", "-");

    InstructionDefs["0fc8-bswap"] = VdtInstr_t("-", NULL);
}
