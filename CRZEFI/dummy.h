#include "definitions.h"

#ifndef _DUMMY_EFI_SHIT_
#define _DUMMY_EFI_SHIT_

EFI_STATUS EFIAPI HookedGetTime(EFI_TIME* time, EFI_TIME_CAPABILITIES* capabilities)
{
    return oGetTime(time, capabilities);
}

EFI_STATUS EFIAPI HookedSetTime(EFI_TIME* time)
{
    return oSetTime(time);
}

EFI_STATUS EFIAPI HookedGetWakeupTime(BOOLEAN* enabled, BOOLEAN* pending, EFI_TIME* time)
{
    return oGetWakeupTime(enabled, pending, time);
}

EFI_STATUS EFIAPI HookedSetWakeupTime(BOOLEAN enable, EFI_TIME* time)
{
    return oSetWakeupTime(enable, time);
}

EFI_STATUS EFIAPI HookedSetVirtualAddressMap(UINTN mapSize, UINTN descriptorSize, UINT32 version, EFI_MEMORY_DESCRIPTOR* virtualMap)
{
    return oSetVirtualAddressMap(mapSize, descriptorSize, version, virtualMap);
}

EFI_STATUS EFIAPI HookedConvertPointer(UINTN debug, void** address)
{
    return oConvertPointer(debug, address);
}

EFI_STATUS EFIAPI HookedGetVariable(CHAR16* variableName, EFI_GUID* vendorGuid, UINT32* attributes, UINTN* dataSize, void* data)
{
    return oGetVariable(variableName, vendorGuid, attributes, dataSize, data);
}

EFI_STATUS EFIAPI HookedSetVariable(CHAR16* variableName, EFI_GUID* vendorGuid, UINT32 attributes, UINTN dataSize, void* data)
{
    return mySetVariable(variableName, vendorGuid, attributes, dataSize, data);
}

EFI_STATUS EFIAPI HookedGetNextVariableName(UINTN* variableNameSize, CHAR16* variableName, EFI_GUID* vendorGuid)
{
    return oGetNextVariableName(variableNameSize, variableName, vendorGuid);
}

EFI_STATUS EFIAPI HookedGetNextHighMonotonicCount(UINT32* highCount)
{
    return oGetNextHighMonotonicCount(highCount);
}

EFI_STATUS EFIAPI HookedResetSystem(EFI_RESET_TYPE resetType, EFI_STATUS resetStatus, UINTN dataSize, CHAR16* resetData)
{
    return oResetSystem(resetType, resetStatus, dataSize, resetData);
}

EFI_STATUS EFIAPI HookedUpdateCapsule(EFI_CAPSULE_HEADER** capsuleHeaderArray, UINTN capsuleCount, EFI_PHYSICAL_ADDRESS scatterGatherList)
{
    return oUpdateCapsule(capsuleHeaderArray, capsuleCount, scatterGatherList);
}

EFI_STATUS EFIAPI HookedQueryCapsuleCapabilities(EFI_CAPSULE_HEADER** capsuleHeaderArray, UINTN capsuleCount, UINT64* maximumCapsuleSize, EFI_RESET_TYPE* resetType)
{
    return oQueryCapsuleCapabilities(capsuleHeaderArray, capsuleCount, maximumCapsuleSize, resetType);
}

EFI_STATUS EFIAPI HookedQueryVariableInfo(UINT32 attributes, UINT64* maximumVariableStorageSize, UINT64* remainingVariableStorageSize, UINT64* maximumVariableSize)
{
    return oQueryVariableInfo(attributes, maximumVariableStorageSize, remainingVariableStorageSize, maximumVariableSize);
}

#endif