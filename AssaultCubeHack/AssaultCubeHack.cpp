// AssaultCubeHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"
using namespace std;
/*
 * No stdafx.h as video
 * base address = entity
 *
 */
int main() {
	//Get Procld of the target process
	DWORD pid = getProcessdId(L"ac_client.exe");

	//Getmodulebaseaddress
	uintptr_t moduleBase = getModuleBaseAddress(pid, L"ac_client.exe");

	//Get Handle to Process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

	//Resolve base address of the pointer chain
	uintptr_t dpBaseAdress = moduleBase + 0x109B74;
	cout << "THe address of Player Entity is: " << hex << dpBaseAdress << endl;

	//Resolve our anmo pointer chain
	vector<unsigned int> ammoOffsets = { 0x150 };
	uintptr_t ammoAdress = findDMAAddy(hProcess, dpBaseAdress, ammoOffsets);
	cout << "THe ammoAdress is: " << hex << ammoAdress << endl;

	//Read Ammo value
	int ammoValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAdress, &ammoValue, sizeof(ammoValue), nullptr);
	cout << "Current ammo:" << dec << ammoValue << endl;

	//Write to it
	int newAmmo = 282;
	WriteProcessMemory(hProcess, (BYTE*)ammoAdress, &newAmmo, sizeof(newAmmo), nullptr);

	// Read again
	ReadProcessMemory(hProcess, (BYTE*)ammoAdress, &newAmmo, sizeof(newAmmo), nullptr);
	cout << "Ammo after:" <<dec<<newAmmo << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
