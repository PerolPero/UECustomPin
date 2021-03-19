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
	// 詳細パネルに作成したプルダウンメニューを表示出来るように登録する
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("CustomPinStruct", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&ICustomPinStructDetail::MakeInstance));
} 
void FCustomPinEd::ShutdownModule() 
{
	// 後始末
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("CustomPinStruct");
} 

#undef LOCTEXT_NAMESPACE