/*
Toradex AG

CoProcLib.h
*/

#ifndef __COPROC_LIB_H__
#define __COPROC_LIB_H__

#include <sys/utsname.h>

#ifdef __cplusplus
extern "C" {
#endif

//wince data type
#ifndef DWORD
#define DWORD unsigned int
#endif

#define CPL_VER_MAJ     2
#define CPL_VER_MIN     0
#define CPL_VER_BUILD   0

// SoC family Type
#define PXA	1
#define TEG	2
#define VF	3
#define CPU_UNKNOWN 0
#define MAGIC_NO	4

//Defines for getPROCID
#define TYPE_PXA270		0x11
#define TYPE_MONAHANS		0x02
#define TYPE_PXA320		0x02
#define TYPE_PXA300		0x08
#define TYPE_PXA310		0x09
#define TYPE_TEGRA2     	0x411FC09
#define TYPE_TEGRA3		0x412FC09
#define TYPE_VYBRID 		0x410FC05
#define TYPE_IMX6		0x412FC09 + MAGIC_NO	/* FIXME */

#define MODULE_FAMILY_COLIBRI	0
#define MODULE_FAMILY_APALIS	1

#define CPU_INFO	"/proc/cpuinfo"
#define SOC_ID		"/sys/bus/soc/devices/soc0/soc_id"

/*
Gets CPUID (Coprocessor 15, cr0)
Processor information
*/
DWORD getCPUID();

/*
Get Processor ID
Use this function to detect on which processor your program is  running
*/
DWORD getPROCID();

/*
Description:      Returns the library Version
Parameter:
Out-Parameter:
- pVerMaj:          Returns the major version number. Set this parameter to NULL if not required.
- pVerMin:          Returns the minor version number. Set this parameter to NULL if not required.
- pBuild:           Returns the build number.         Set this parameter to NULL if not required.
Return Value:		No return value.
*/
void CPLGetLibVersion(DWORD *pVerMaj, DWORD *pVerMin, DWORD *pBuild);

//-------------------------------------------------------------------------
/// To findout SoC family type
/// @param[out] CPU_TYPE: choose between soc family i.e PXA or tegra
DWORD GetSoCType();

/*
Returns
CPU_TYPE: MODULE_FAMILY_COLIBRI or MODULE_FAMILY_APALIS
*/
DWORD getModuleFamily(char *moduleName);

#ifdef __cplusplus
}
#endif

#endif
