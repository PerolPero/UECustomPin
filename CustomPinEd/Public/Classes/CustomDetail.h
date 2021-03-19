// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCustomizations.h"
#include "IPropertyTypeCustomization.h"
//#include "Widgets/Input/STextComboBox.h"
#include "CustomTextComboBox.h"
#include "Styling/SlateTypes.h"

class CUSTOMPINED_API ICustomDetailBase : public IPropertyTypeCustomization
{
public:
	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	TSharedPtr<IPropertyHandle> KeyHandle;
	TSharedPtr<SCustomTextComboBox> KeyComboBox;

	void OnStateValueChanged(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo);
	void OnStateListOpened();
	void OnCheckStateChanged(ECheckBoxState CheckState);

	TSharedPtr<class IPropertyUtilities> PropertyUtilities;

protected:
	TArray<TSharedPtr<FString>> DisplayStrings;

	virtual void SetDisplayStrings() = 0;
};


// FCustomPinStructにアクセスする用
class CUSTOMPINED_API ICustomPinStructDetail : public ICustomDetailBase
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
		return MakeShareable(new ICustomPinStructDetail());
	}

protected:
	virtual void SetDisplayStrings() override;
};