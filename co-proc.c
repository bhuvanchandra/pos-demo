#include "co-proc-lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-------------------------------------------------------------------------
/// To findout SoC family type
/// @param[out] CPU_TYPE: choose between soc family i.e PXA or tegra
DWORD GetSoCType()
{
	DWORD CPUtype = CPU_UNKNOWN;
	if((getPROCID() == TYPE_PXA320)|
		(getPROCID() == TYPE_PXA300)|
		(getPROCID() == TYPE_PXA310)|
		(getPROCID() == TYPE_PXA270)){
		CPUtype = PXA;
	}
	else if((getPROCID() == TYPE_TEGRA2)|
		(getPROCID() == TYPE_TEGRA3)){
		CPUtype = TEG;
	}
	else if (getPROCID() == TYPE_VYBRID){
		CPUtype = VF;
	}
	return CPUtype;
}

DWORD getCPUID(){
	static unsigned int cpu_id = 0; /* this will stay for as long as the programm is executing */

	if(cpu_id == 0){ /* first time this gets calculated */
		char buf[256];
		char *numStr;
		FILE *file;
		unsigned int res;

		/* CPU type */
		/*
		 012345678901234567890123456789
		 CPU implementer : 0x41
		 CPU architecture: 7
		 CPU variant     : 0x1
		 CPU part        : 0xc09
		 CPU revision    : 0
		 */
		file = fopen(CPU_INFO, "r");
		if (file){
			while (fgets(buf, 255, file)) {
				if (strstr(buf, "CPU implementer")){
					numStr = strstr(&buf[14], ": ");
					(void)sscanf(&numStr[4], "%x", &res);
					cpu_id = (res & 0xff) << 24;
				}
				else if (strstr(buf, "CPU variant")){
					numStr = strstr(&buf[10], ": ");
					(void)sscanf(&numStr[4], "%x", &res);
					cpu_id |= (((res <<4)  & 0xf0) | 0xf) << 16;
				}
				else if (strstr(buf, "CPU part")){
					numStr = strstr(&buf[8], ": ");
					(void)sscanf(&numStr[4], "%x", &res);
					cpu_id |= (res & 0xfff) << 4;
				}
				else if (strstr(buf, "CPU revision")){
					numStr = strstr(&buf[11], ": ");
					(void)sscanf(&numStr[2], "%u", &res);
					cpu_id |= (res & 0xf) ;
				}
			}
			fclose(file);
		}
		else{
			printf("CPU info not available!");
			exit(-1);
		}
		cpu_id >>= 4;

		file = fopen(SOC_ID, "r");
		if(file){
			while(fgets(buf, 255, file)){
				if(strstr(buf, "i.MX6")){	/* Check for the id */
					cpu_id += MAGIC_NO; /* If ID matches to i.MX6xx we are adding constant to cpuID inorder to make it unique:FIXME */
				}
			}
			fclose(file);
		}
	}
	return cpu_id; /* subsequent calls just return the pre-calculated value */
}

DWORD getPROCID(){
	DWORD procID;
	procID = getCPUID();

	//procID = procID>>4; 			//Remove Stepping
	if((((procID >> 4) >> 12) & 0x0f) == 5){	//PXA
		return (procID & 0x03F); 	// Return only Product ID
	}
	else{
		return procID; 			//return processor ID
	}
}

void CPLGetLibVersion(DWORD *pVerMaj, DWORD *pVerMin, DWORD *pBuild){
    if(pVerMaj)
        *pVerMaj = CPL_VER_MAJ;
    if(pVerMin)
        *pVerMin = CPL_VER_MIN;
    if(pBuild)
        *pBuild  = CPL_VER_BUILD;
}

DWORD getModuleFamily(char *moduleName){
#if 0
	uname(&uNameData);
	if(strstr(uNameData.nodename, "colibri")){
		return MODULE_FAMILY_COLIBRI;
	}
	else if(strstr(uNameData.nodename, "apalis")){
		return MODULE_FAMILY_APALIS;
	}
#endif

	char buf[256], hardware[128] = {0};
	FILE *file = NULL;
	char *start = NULL, *numStr = NULL, *end = NULL;
	size_t len = 0;

	file = fopen(CPU_INFO, "r");	/* Used for non-dt kernels */
	if (file){
		while (fgets(buf, 255, file)) {
			if(strstr(buf, "Hardware")){
				numStr = strstr(&buf[7], ":");
				sprintf(hardware, "%s", numStr);
			}
		}
		fclose(file);
		start = strstr(hardware, "Toradex");
		if (start != NULL) {
			start += 8;
			end = strstr(start, "Module");
			if(end) len = end - start - 1;
			else len = strcspn(start, "\n");
		} else {
			file = fopen("/sys/firmware/devicetree/base/model", "r");	/* e.g: Toradex Colibri VF61 on Colibri Evaluation Board */
			if (file == NULL) {
				file = fopen("/sys/bus/soc/devices/soc0/machine", "r");	/* e.g: Toradex Apalis iMX6Q on Apalis Evaluation Board */
			}

			if (file) {
				while(fgets(buf, 255, file)){
					start = strstr(buf, "Toradex");
					if (start == NULL) {
						printf("Invalid Machine!");
						exit(-1);
					}
					start += 8;
					end = strstr(start, "on");
					if (end) len = end - start - 1;
					else len = strlen(start);
				}
				fclose(file);
			}
		}
	} else {
		printf("CPU info not available!");
		exit(-1);
	}

	strncpy(moduleName, start, len);	/* for showing the module name on window title */
	if (strstr(moduleName, "Colibri")) {
		return MODULE_FAMILY_COLIBRI;
	} else if (strstr(moduleName, "Apalis")) {
		return MODULE_FAMILY_APALIS;
	}

	return -1;
}
