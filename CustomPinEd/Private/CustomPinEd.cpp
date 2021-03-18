// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved. 

#include "CustomPinEd/Public/CustomPinEd.h"
#include "Modules/ModuleManager.h"
#include "CustomDetail.h"
#include "PropertyEditorModule.h"


class FCustomPinEd : public ICustomPinEd 
{ 
	/** IModuleInterface implementation */ 
	virtual void StartupModule() override; 
	virtual void ShutdownModule() override; 
}; 
IMPLEMENT_MODULE( FCustomPinEd, CustomPinEd )


#define LOCTEXT_NAMESPACE "CustomPinEd"


void FCustomPinEd::StartupModule() 
{
	// �ڍ׃p�l���ɍ쐬�����v���_�E�����j���[��\���o����悤�ɓo�^����
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("CustomPinStruct", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ICustomPinStructDetail::MakeInstance));
} 
void FCustomPinEd::ShutdownModule() 
{
	// ��n��
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("CustomPinStruct");
} 

#undef LOCTEXT_NAMESPACE