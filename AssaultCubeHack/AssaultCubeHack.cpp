// AssaultCubeHack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"
using namespace std;
void changeMemoValue();
//void keyPatch();


/*
 * No stdafx.h as video
 * base address = entity
 *
 */
int main() {
	HANDLE hProcess = 0;
	uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;
	const int newValue = 1337;
	DWORD pid = getProcessdId(L"ac_client.exe");
	if (pid) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
		moduleBase = getModuleBaseAddress(pid, L"ac_client.exe");
		localPlayerPtr = moduleBase + 0x10f4f4;
		healthAddr = findDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
	} else {
		std::cout << "Process not found, press enter to exit\n";
		//getchar();
		return 0;
	}

	DWORD dwExit = 0;
	cout << "Key listening..." << endl;
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {
		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bHealth = !bHealth;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			cout << "Ammo" << endl;
			bAmmo = !bAmmo;
			if (bAmmo)
				mem::patchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
			else
				mem::patchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);

		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
			cout << "Recoil" << endl;
			bRecoil = !bRecoil;
			if (bRecoil) {
				mem::nopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
			} else {
				mem::patchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
			}
		}

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			return 0;
		}

		Sleep(1000);
	}
	cout << "Process not found!" << endl;
	//getchar();
	return 0;
}

void changeMemoValue() {
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
	cout << "Ammo after:" << dec << newAmmo << endl;

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
