
#ifndef _SUPER_EFI_INCLUDE_
#define _SUPER_EFI_INCLUDE_

#define GNU_EFI_USE_MS_ABI 1

#define MicrosoftCallingType __attribute__((ms_abi))

#include <efi.h>
#include <efilib.h>

// Dummy protocol struct
typedef struct _DummyProtocalData {
    UINTN blank;
} DummyProtocalData;

typedef unsigned long long ptr64;

// Pointers to original functions
extern EFI_GET_TIME oGetTime;
extern EFI_SET_TIME oSetTime;
extern EFI_GET_WAKEUP_TIME oGetWakeupTime;
extern EFI_SET_WAKEUP_TIME oSetWakeupTime;
extern EFI_SET_VIRTUAL_ADDRESS_MAP oSetVirtualAddressMap;
extern EFI_CONVERT_POINTER oConvertPointer;
extern EFI_GET_VARIABLE oGetVariable;
extern EFI_SET_VARIABLE oSetVariable;
extern EFI_GET_NEXT_VARIABLE_NAME oGetNextVariableName;
extern EFI_GET_NEXT_HIGH_MONO_COUNT oGetNextHighMonotonicCount;
extern EFI_RESET_SYSTEM oResetSystem;
extern EFI_UPDATE_CAPSULE oUpdateCapsule;
extern EFI_QUERY_CAPSULE_CAPABILITIES oQueryCapsuleCapabilities;
extern EFI_QUERY_VARIABLE_INFO oQueryVariableInfo;

// Struct containing data used to communicate with the client
typedef struct _MemoryCommand {
    int magic;
    int operation;
    ptr64 data[6];
} MemoryCommand;

// Functions (Windows only)
typedef int (MicrosoftCallingType* PsLookupProcessByProcessId)(
    void* ProcessId,
    void* OutPEProcess
    );
typedef void* (MicrosoftCallingType* PsGetProcessSectionBaseAddress)(
    void* PEProcess
    );
typedef int (MicrosoftCallingType* MmCopyVirtualMemory)(
    void* SourceProcess,
    void* SourceAddress,
    void* TargetProcess,
    void* TargetAddress,
    ptr64 BufferSize,
    char PreviousMode,
    void* ReturnSize
    );

EFI_STATUS RunCommand(MemoryCommand* cmd);
EFI_STATUS EFIAPI mySetVariable(IN CHAR16* VariableName, IN EFI_GUID* VendorGuid, IN UINT32 Attributes, IN UINTN DataSize, IN VOID* Data);
VOID EFIAPI SetVirtualAddressMapEvent(IN EFI_EVENT Event, IN VOID* Context);
VOID EFIAPI ExitBootServicesEvent(IN EFI_EVENT Event, IN VOID* Context);
VOID* SetServicePointer(IN OUT EFI_TABLE_HEADER* ServiceTableHeader, IN OUT VOID** ServiceTableFunction, IN VOID* NewFunction);
EFI_STATUS EFI_FUNCTION efi_unload(IN EFI_HANDLE ImageHandle);
EFI_STATUS efi_main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);


#endif
