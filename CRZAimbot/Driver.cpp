#include "Driver.h"


HANDLE Driver::driverH = 0;
uintptr_t Driver::currentProcessId = 0;
GUID DummyGuid = { 2 };
mRtlAdjustPrivilege myRtlAdjustPrivilege = (mRtlAdjustPrivilege)NULL;
mNtSetSystemEnvironmentValueEx myNtSetSystemEnvironmentValueEx = (mNtSetSystemEnvironmentValueEx)NULL;
mNtQuerySystemInformation myNtQuerySystemInformation = (mNtQuerySystemInformation)NULL;

NTSTATUS SetSystemEnvironmentPrivilege(BOOLEAN Enable, PBOOLEAN WasEnabled)
{
	if (WasEnabled != nullptr)
		*WasEnabled = FALSE;

	BOOLEAN SeSystemEnvironmentWasEnabled;
	const NTSTATUS Status = myRtlAdjustPrivilege(SE_SYSTEM_ENVIRONMENT_PRIVILEGE,
		Enable,
		FALSE,
		&SeSystemEnvironmentWasEnabled);

	SLog(std::wstring(L"Privilege status: " + std::to_wstring((DWORD)Status)).c_str());

	if (NT_SUCCESS(Status) && WasEnabled != nullptr)
		*WasEnabled = SeSystemEnvironmentWasEnabled;

	return Status;
}

void Driver::SendCommand(MemoryCommand* cmd) {
	Protect(_ReturnAddress());
    wchar_t VarName[] = { 'F','a','s','t','B','o','o','t','O','p','t','i','o','n','\0' };
	UNICODE_STRING FVariableName = UNICODE_STRING();
	FVariableName.Buffer = VarName;
	FVariableName.Length = 28;
	FVariableName.MaximumLength = 30;
	//UNICODE_STRING VariableName2 = { sizeof(VARIABLE_NAME) - sizeof(VARIABLE_NAME[0]),sizeof(VARIABLE_NAME),(PWSTR)VARIABLE_NAME };
	//UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
	myNtSetSystemEnvironmentValueEx(
		&FVariableName,
		&DummyGuid,
		cmd,
		sizeof(MemoryCommand),
		ATTRIBUTES);
	memset(VarName, 0, sizeof(VarName));
	//memset(VariableName.Buffer, 0, VariableName.Length);
	//VariableName.Length = 0;
	Unprotect(_ReturnAddress());
}


uintptr_t Driver::GetBaseAddress(uintptr_t pid) {
	Protect(_ReturnAddress());
	uintptr_t result = 0;
	MemoryCommand cmd = MemoryCommand();
	cmd.operation = baseOperation * 0x289;
	cmd.magic = COMMAND_MAGIC;
	cmd.data[0] = pid;
	cmd.data[1] = (uintptr_t)&result;
	Unprotect(SendCommand);
	SendCommand(&cmd);
	Protect(SendCommand);
	Unprotect(_ReturnAddress());
	return result;
}

NTSTATUS Driver::copy_memory(
	const uintptr_t	src_process_id,
	const uintptr_t src_address,
	const uintptr_t	dest_process_id,
	const uintptr_t	dest_address,
	const size_t	size) {
	Protect(_ReturnAddress());
	uintptr_t result = 0;
	MemoryCommand cmd = MemoryCommand();
	cmd.operation = baseOperation * 0x823;
	cmd.magic = COMMAND_MAGIC;
	cmd.data[0] = (uintptr_t)src_process_id;
	cmd.data[1] = (uintptr_t)src_address;
	cmd.data[2] = (uintptr_t)dest_process_id;
	cmd.data[3] = (uintptr_t)dest_address;
	cmd.data[4] = (uintptr_t)size;
	cmd.data[5] = (uintptr_t)&result;
	Unprotect(SendCommand);
	SendCommand(&cmd);
	Protect(SendCommand);
	Unprotect(_ReturnAddress());
	return (NTSTATUS)result;
}

uintptr_t GetKernelModuleExport(uintptr_t kernel_module_base, char* function_name)
{
	if (!kernel_module_base)
		return 0;

	IMAGE_DOS_HEADER dos_header = { 0 };
	IMAGE_NT_HEADERS64 nt_headers = { 0 };

	Unprotect(Driver::read_memory);
	Driver::read_memory(4, kernel_module_base, (uintptr_t)&dos_header, sizeof(dos_header));
	Protect(Driver::read_memory);
	if (dos_header.e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	Unprotect(Driver::read_memory);
	Driver::read_memory(4, kernel_module_base + dos_header.e_lfanew, (uintptr_t)&nt_headers, sizeof(nt_headers));
	Protect(Driver::read_memory);
	if (nt_headers.Signature != IMAGE_NT_SIGNATURE)
		return 0;

	const auto export_base = nt_headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	const auto export_base_size = nt_headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	if (!export_base || !export_base_size)
		return 0;

	const auto export_data = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(VirtualAlloc(nullptr, export_base_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

	Unprotect(Driver::read_memory);
	Driver::read_memory(4, kernel_module_base + export_base, (uintptr_t)export_data, export_base_size);
	Protect(Driver::read_memory);
	//if (!ReadMemory(device_handle, kernel_module_base + export_base, export_data, export_base_size))
	//{
	//	VirtualFree(export_data, 0, MEM_RELEASE);
	//	return 0;
	//}

	const auto delta = reinterpret_cast<uintptr_t>(export_data) - export_base;

	const auto name_table = reinterpret_cast<UINT32*>(export_data->AddressOfNames + delta);
	const auto ordinal_table = reinterpret_cast<UINT16*>(export_data->AddressOfNameOrdinals + delta);
	const auto function_table = reinterpret_cast<UINT32*>(export_data->AddressOfFunctions + delta);

	for (auto i = 0u; i < export_data->NumberOfNames; ++i)
	{
		char * current_function_name = (char*)(name_table[i] + delta);

		if (!_stricmp(current_function_name, function_name))
		{
			const auto function_ordinal = ordinal_table[i];
			const auto function_address = kernel_module_base + function_table[function_ordinal];

			if (function_address >= kernel_module_base + export_base && function_address <= kernel_module_base + export_base + export_base_size)
			{
				VirtualFree(export_data, 0, MEM_RELEASE);
				return 0; // No forwarded exports on 64bit?
			}

			VirtualFree(export_data, 0, MEM_RELEASE);
			return function_address;
		}
	}

	VirtualFree(export_data, 0, MEM_RELEASE);
	return 0;
}

uintptr_t GetKernelModuleAddress(char* module_name)
{
	void* buffer = nullptr;
	DWORD buffer_size = 0;

	NTSTATUS status = myNtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(SystemModuleInformation), buffer, buffer_size, &buffer_size);

	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		VirtualFree(buffer, 0, MEM_RELEASE);

		buffer = VirtualAlloc(nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (buffer == 0){
			return 0;
		}
		status = myNtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(SystemModuleInformation), buffer, buffer_size, &buffer_size);
	}

	if (!NT_SUCCESS(status))
	{
		VirtualFree(buffer, 0, MEM_RELEASE);
		return 0;
	}

	const PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)buffer;
	if (modules == nullptr) {
		VirtualFree(buffer, 0, MEM_RELEASE);
		return 0;
	}
	for (auto i = 0u; i < modules->NumberOfModules; ++i)
	{
		char* current_module_name = (char*)(modules->Modules[i].FullPathName + modules->Modules[i].OffsetToFileName);

		if (!_stricmp(current_module_name, module_name))
		{
			const uintptr_t result = (uintptr_t)(modules->Modules[i].ImageBase);

			VirtualFree(buffer, 0, MEM_RELEASE);
			return result;
		}
	}

	VirtualFree(buffer, 0, MEM_RELEASE);
	return 0;
}

bool Driver::initialize() {
	Protect(_ReturnAddress());
	currentProcessId = GetCurrentProcessId();
	BOOLEAN SeSystemEnvironmentWasEnabled;
	SLog(L"Driver Init");
	HMODULE ntmodule = LoadLibraryW(L"ntdll.dll");
	SLog(std::wstring(L"loaded ntdll "+ std::to_wstring((DWORD)ntmodule) +L" p "+ std::to_wstring(currentProcessId)).c_str());
	BYTE ntqsi[] = { 'N','t','Q','u','e','r','y','S','y','s','t','e','m','I','n','f','o','r','m','a','t','i','o','n',0 };
	BYTE nssevex[] = { 'N','t','S','e','t','S','y','s','t','e','m','E','n','v','i','r','o','n','m','e','n','t','V','a','l','u','e','E','x',0 };
	BYTE rtlajp[] = { 'R','t','l','A','d','j','u','s','t','P','r','i','v','i','l','e','g','e',0 };
	myNtQuerySystemInformation = (mNtQuerySystemInformation)GetProcAddress(ntmodule, (char*)ntqsi);
	myNtSetSystemEnvironmentValueEx = (mNtSetSystemEnvironmentValueEx)GetProcAddress(ntmodule, (char*)nssevex);
	myRtlAdjustPrivilege = (mRtlAdjustPrivilege)GetProcAddress(ntmodule, (char*)rtlajp);
	//memset(ntqsi, 0, sizeof(ntqsi));
	//memset(nssevex, 0, sizeof(nssevex));
	//memset(rtlajp, 0, sizeof(rtlajp));

	SLog(std::wstring(L"Funcs 1 " + std::to_wstring((uintptr_t)myNtQuerySystemInformation) + L" 2 " + std::to_wstring((uintptr_t)myNtSetSystemEnvironmentValueEx) + L" 3 " + std::to_wstring((uintptr_t)myRtlAdjustPrivilege)).c_str());

	Unprotect(SetSystemEnvironmentPrivilege);
	NTSTATUS status = SetSystemEnvironmentPrivilege(true, &SeSystemEnvironmentWasEnabled);
	Protect(SetSystemEnvironmentPrivilege);

	if (!NT_SUCCESS(status)) {
		Unprotect(_ReturnAddress());
		return false;
	}

	SLog(L"Cleared old");

	Unprotect(GetKernelModuleAddress);
	BYTE nstosname[] = {'n','t','o','s','k','r','n','l','.','e','x','e',0};
	uintptr_t kernelModuleAddress = GetKernelModuleAddress((char*)nstosname);
	memset(nstosname, 0, sizeof(nstosname));
	SLog(std::wstring(L"kernel module addr " + std::to_wstring((uintptr_t)kernelModuleAddress)).c_str());
	Protect(GetKernelModuleAddress);
	
	Unprotect(GetKernelModuleExport);
	BYTE pbid[] = { 'P','s','L','o','o','k','u','p','P','r','o','c','e','s','s','B','y','P','r','o','c','e','s','s','I','d',0 };
	BYTE gba[] = { 'P','s','G','e','t','P','r','o','c','e','s','s','S','e','c','t','i','o','n','B','a','s','e','A','d','d','r','e','s','s',0 };
	BYTE mmcp[] = { 'M','m','C','o','p','y','V','i','r','t','u','a','l','M','e','m','o','r','y',0 };
	uintptr_t kernel_PsLookupProcessByProcessId = GetKernelModuleExport(kernelModuleAddress, (char*)pbid);
	uintptr_t kernel_PsGetProcessSectionBaseAddress = GetKernelModuleExport(kernelModuleAddress, (char*)gba);
	uintptr_t kernel_MmCopyVirtualMemory = GetKernelModuleExport(kernelModuleAddress, (char*)mmcp);
	SLog(std::wstring(L"exports " + std::to_wstring((uintptr_t)kernel_PsLookupProcessByProcessId) + L" - " + std::to_wstring((uintptr_t)kernel_PsGetProcessSectionBaseAddress)+ L" - " + std::to_wstring((uintptr_t)kernel_MmCopyVirtualMemory)).c_str());
	memset(pbid, 0, sizeof(pbid));
	memset(gba, 0, sizeof(gba));
	memset(mmcp, 0, sizeof(mmcp));
	Protect(GetKernelModuleExport);
	
	uintptr_t result = 0;
	MemoryCommand cmd = MemoryCommand();
	cmd.operation = baseOperation * 0x612;
	cmd.magic = COMMAND_MAGIC;
	cmd.data[0] = kernel_PsLookupProcessByProcessId;
	cmd.data[1] = kernel_PsGetProcessSectionBaseAddress;
	cmd.data[2] = kernel_MmCopyVirtualMemory;
	cmd.data[3] = (uintptr_t)&result;
	Unprotect(SendCommand);
	SLog(L"Sending config");
	SendCommand(&cmd);
	SLog(L"configured");
	Protect(SendCommand);
	
	Unprotect(_ReturnAddress());
	return result;
}

NTSTATUS Driver::read_memory(
	const uintptr_t	process_id,
	const uintptr_t address,
	const uintptr_t buffer,
	const size_t	size) {
	Protect(_ReturnAddress());
	Unprotect(copy_memory);
	NTSTATUS result = copy_memory(process_id, address, currentProcessId, buffer, size);
	Protect(copy_memory);
	Unprotect(_ReturnAddress());
	return result;
}

NTSTATUS Driver::write_memory(
	const uintptr_t	process_id,
	const uintptr_t address,
	const uintptr_t buffer,
	const size_t	size) {
	Protect(_ReturnAddress());
	Unprotect(copy_memory);
	NTSTATUS result = copy_memory(currentProcessId, buffer, process_id, address, size);
	Protect(copy_memory);
	Unprotect(_ReturnAddress());
	return result;
}
