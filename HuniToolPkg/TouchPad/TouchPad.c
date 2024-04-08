#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/I2cMaster.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

EFI_STATUS
EFIAPI
I2cBusComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

EFI_STATUS
EFIAPI
I2cBusComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mI2cBusDriverNameTable[] = {
  { "eng;en", (CHAR16 *) L"I2C Bus Driver" },
  { NULL , NULL }
};


GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL  gI2cBusComponentName = {
  (EFI_COMPONENT_NAME_GET_DRIVER_NAME) I2cBusComponentNameGetDriverName,
  (EFI_COMPONENT_NAME_GET_CONTROLLER_NAME) I2cBusComponentNameGetControllerName,
  "eng"
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL gI2cBusComponentName2 = {
  I2cBusComponentNameGetDriverName,
  I2cBusComponentNameGetControllerName,
  "en"
};


EFI_STATUS
EFIAPI
I2cBusComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mI2cBusDriverNameTable,
           DriverName,
           (BOOLEAN)(This != &gI2cBusComponentName2)
           );
}


EFI_STATUS
EFIAPI
I2cBusComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME2_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  )
{
  return EFI_UNSUPPORTED;
}




EFI_GUID    gEfiDriverBindingProtocolGuid                = {0x27D61E65, 0xFF1A, 0x465B, {0x9e, 0xfa, 0x71, 0x42, 0xf1, 0xd2, 0x8e, 0x22}};

EFI_I2C_MASTER_PROTOCOL *I2cMaster;


typedef struct {
  EFI_HANDLE ControllerHandle;
  UINTN       SlaveAddress;
  EFI_I2C_REQUEST_PACKET    *RequestPacket;
  EFI_STATUS                                       Status;
} I2C_TOUCHPAD_DRIVER_DATA;

EFI_STATUS EFIAPI Supported(
  EFI_DRIVER_BINDING_PROTOCOL *This,
  EFI_HANDLE ControllerHandle,
  EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath
) {
    EFI_STATUS Status;
    Status = gBS->OpenProtocol(
    ControllerHandle,
    &gEfiI2cMasterProtocolGuid,
    NULL,
    This->DriverBindingHandle,
    ControllerHandle,
    EFI_OPEN_PROTOCOL_TEST_PROTOCOL
  );

  return Status;
}

EFI_STATUS EFIAPI Start(
  EFI_DRIVER_BINDING_PROTOCOL *This,
  EFI_HANDLE ControllerHandle,
  EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath
) {
  I2C_TOUCHPAD_DRIVER_DATA *DriverData;
  EFI_STATUS               Status;
  UINTN                    Frequency = 400000;

  Status = gBS->AllocatePool(
    EfiBootServicesData,
    sizeof(I2C_TOUCHPAD_DRIVER_DATA),
    (VOID **)&DriverData
  );
  if (EFI_ERROR(Status)) {
    Print(L"Failed to allocate memory for driver data: %r\n", Status);
    return Status;
  }

  DriverData->ControllerHandle = ControllerHandle;
  DriverData->SlaveAddress = 0x15;


  Status = I2cMaster->SetBusFrequency(I2cMaster, &Frequency); // 设置I2C总线时钟速率为400kHz
  if (EFI_ERROR(Status)) {
    Print(L"Failed to set I2C bus frequency: %r\n", Status);
    return Status;
  }
  
//   UINT8 InitCommand[] = {0x01, 0x02, 0x03}; // 示例初始化命令
//   Status = I2cMaster->StartRequest(I2cMaster, &DriverData->SlaveAddress, 0, InitCommand, sizeof(InitCommand), NULL, 0);
//   if (EFI_ERROR(Status)) {
//     Print(L"Failed to send initialization command to touchpad: %r\n", Status);
//     return Status;
//   }
  

  Status = I2cMaster->StartRequest(I2cMaster, DriverData->SlaveAddress, DriverData->RequestPacket, NULL, &DriverData->Status);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to read touchpad configuration: %r\n", Status);
    return Status;
  }


  return EFI_SUCCESS;
}

EFI_STATUS EFIAPI Stop(
  EFI_DRIVER_BINDING_PROTOCOL *This,
  EFI_HANDLE ControllerHandle,
  UINTN NumberOfChildren,
  EFI_HANDLE *ChildHandleBuffer
) {
    
    Print(L"Don't stop");
  return EFI_SUCCESS;
}

EFI_DRIVER_BINDING_PROTOCOL gI2cTouchpadDriverBinding = {
  Supported,
  Start,
  Stop,
  0x10,
  NULL,
  NULL
};

EFI_STATUS EFIAPI DriverEntry(
  EFI_HANDLE ImageHandle,
  EFI_SYSTEM_TABLE *SystemTable
) {
    EFI_STATUS Status;

  Status = gBS->LocateProtocol(&gEfiI2cMasterProtocolGuid, NULL, (VOID **)&I2cMaster);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to locate I2C Master protocol: %r\n", Status);
    return Status;
  }

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gI2cTouchpadDriverBinding,
             NULL,
             &gI2cBusComponentName,
             &gI2cBusComponentName2
             );
  if (EFI_ERROR(Status)) {
    Print(L"Failed to install driver binding protocol: %r\n", Status);
    return Status;
  }

  return EFI_SUCCESS;
}