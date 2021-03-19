#include "CustomTextComboBox.h"
#include "Widgets/Input/SSearchBox.h"

void SCustomTextComboBox::Construct(const FArguments& InArgs)
{
    SelectionChanged = InArgs._OnSelectionChanged;
    GetTextLabelForItem = InArgs._OnGetTextLabelForItem;
    Font = InArgs._Font;

    // ウィジェットの作成
    this->ChildSlot
    [
        SAssignNew(StringCombo, SCustomComboBox)
        .ComboBoxStyle(InArgs._ComboBoxStyle)
        .ButtonStyle(InArgs._ButtonStyle)
        .OptionsSource(InArgs._OptionsSource)
        .OnGenerateWidget(this, &SCustomTextComboBox::MakeItemWidget)
        .OnSelectionChanged(this, &SCustomTextComboBox::OnSelectionChanged)
        .OnComboBoxOpening(InArgs._OnComboBoxOpening)
        .InitiallySelectedItem(InArgs._InitiallySelectedItem)
        .ContentPadding(InArgs._ContentPadding)
        [
            SNew(STextBlock)
            .ColorAndOpacity(InArgs._ColorAndOpacity)
            .Text(this, &SCustomTextComboBox::GetSelectedTextLabel)
            .Font(InArgs._Font)
        ]
    ];
    SelectedItem = StringCombo->GetSelectedItem();
}

FText SCustomTextComboBox::GetItemTextLabel(TSharedPtr<FString> StringItem) const
{
    if (!StringItem.IsValid())
    {
        return FText::GetEmpty();
    }

    return FText::FromString(
        (GetTextLabelForItem.IsBound())
        ? GetTextLabelForItem.Execute(StringItem)
        : *StringItem
    );
}

FText SCustomTextComboBox::GetSelectedTextLabel() const
{
    TSharedPtr<FString> StringItem = StringCombo->GetSelectedItem();
    return GetItemTextLabel(StringItem);
}

TSharedRef<SWidget> SCustomTextComboBox::MakeItemWidget(TSharedPtr<FString> StringItem)
{
    check(StringItem.IsValid());

    return  SNew(STextBlock)
            .Text(this, &SCustomTextComboBox::GetItemTextLabel, StringItem)
            .Font(Font);
}

void SCustomTextComboBox::OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo)
{
    if (Selection.IsValid())
    {
        SelectedItem = Selection;
    }
    SelectionChanged.ExecuteIfBound(Selection, SelectInfo);
}

void SCustomTextComboBox::SetSelectedItem(TSharedPtr<FString> NewSelection)
{
    StringCombo->SetSelectedItem(NewSelection);
}

void SCustomTextComboBox::RefreshOptions()
{
    StringCombo->RefreshOptions();
}

void SCustomTextComboBox::ClearSelection()
{
    StringCombo->ClearSelection();
}