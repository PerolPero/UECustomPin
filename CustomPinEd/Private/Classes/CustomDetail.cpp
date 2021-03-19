// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomDetail.h"
#include "Widgets/Input/SCheckBox.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IPropertyUtilities.h"

void ICustomDetailBase::CustomizeHeader(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructPropertyHandle = inStructPropertyHandle;

	DisplayStrings.Add(MakeShareable<FString>(new FString("None")));

	// 派生先で文字列の一覧を取得してくる
	SetDisplayStrings();

	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);

	FName Key;
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex) 
	{
		const TSharedPtr< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex);
		if (ChildHandle->GetProperty()->GetName() == TEXT("Key")) 
		{
			KeyHandle = ChildHandle;
			ChildHandle->GetValue(Key);
		}
	}
	check(KeyHandle.IsValid());

	// 取得してきたKeyがリストの中にあるかチェック
	int Index = 0;
	bool Found = false;
	for (int32 i = 0; i < DisplayStrings.Num(); ++i) 
	{
		if (DisplayStrings[i]->Equals(Key.ToString())) 
		{
			Index = i;
			Found = true;
			break;
		}
	}
	if (!Found) 
	{
		Key = TEXT("None");
		KeyHandle->SetValue(Key);
		UE_LOG(LogTemp, Error, TEXT("ICustomDetailBase: %s 設定していた文字列が一覧から見つかりませんでした。再指定して下さい"), *Key.ToString());
	}

	HeaderRow
	.NameContent()
	[
		StructPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(500)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		[
			SAssignNew(KeyComboBox, SCustomTextComboBox)
			.OptionsSource(&DisplayStrings)
		    .OnSelectionChanged(this, &ICustomDetailBase::OnStateValueChanged)
		    .InitiallySelectedItem(DisplayStrings[Index])
		]
	];
}

void ICustomDetailBase::CustomizeChildren(TSharedRef<class IPropertyHandle> inStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}

void ICustomDetailBase::OnStateValueChanged(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo)
{
	if (ItemSelected.IsValid()) 
	{
		if (DisplayStrings.Find(ItemSelected)) 
		{
			KeyHandle->SetValue(FName(**ItemSelected));
		}
	}
}

void ICustomDetailBase::OnStateListOpened()
{
}

void ICustomPinStructDetail::SetDisplayStrings()
{
	/** とりあえず仮で３つのリストを作る */
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("test"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("TEST"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("Test"))));

	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("01 Y2064.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("02 Day1.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("03 D.O.L.L.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("04 X5.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("05 Machines Are Talking.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("06 Safety First.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("07 Safety First.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("08 Greenlight.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("09 Greenlight.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("10 Beacon.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("11 Beacon.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("12 Sentry.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("13 Sentry.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("14 Make Sense.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("15 Horizon.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("16 Initialize.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("17 For the Record.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("18 For the Record.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("19 Host .a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("20 Host .b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("21 Nihilism.a.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("22 Nihilism.b.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("23 PROUD OF YOU.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("24 Cuty.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("25 Made in Heaven.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("26 Departure.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("27 Black Out.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("28 White Front.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("29 To The TwilighT.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("30 Snowflake.m4a"))));
	DisplayStrings.Add(MakeShareable<FString>(new FString(TEXT("31 What Am I Fighting For.m4a"))));
}