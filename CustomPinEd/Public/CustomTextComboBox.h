#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Layout/Margin.h"
#include "Styling/SlateColor.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Framework/SlateDelegates.h"
#include "CustomComboBox.h"

/**
 * テキストコンテンツを表示するコンボボックスです。
 */
class CUSTOMPINED_API SCustomTextComboBox : public SCompoundWidget {
public:

	DECLARE_DELEGATE_RetVal_OneParam(FString, FGetTextComboLabel, TSharedPtr<FString>);
	typedef TSlateDelegates< TSharedPtr<FString> >::FOnSelectionChanged FOnTextSelectionChanged;

	SLATE_BEGIN_ARGS(SCustomTextComboBox)
		: _ComboBoxStyle(&FCoreStyle::Get().GetWidgetStyle< FComboBoxStyle >("ComboBox"))
		, _ButtonStyle(nullptr)
		, _ColorAndOpacity(FSlateColor::UseForeground())
		, _ContentPadding(FMargin(4.0, 2.0))
		, _OnGetTextLabelForItem() {
	}

	SLATE_STYLE_ARGUMENT(FComboBoxStyle, ComboBoxStyle)

	/** コンボボックスのボタン部分のビジュアルスタイル（ComboBoxStyleをオーバーライドします） */
	SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)

	/** 選択できる文字列 */
	SLATE_ARGUMENT(TArray< TSharedPtr<FString> >*, OptionsSource)

	/** テキストの色と不透明度 */
	SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

	/** テキストの描画に使用するフォントを設定する */
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)

	/** コンボボックスのボタンコンテンツのビジュアルパディング */
	SLATE_ATTRIBUTE(FMargin, ContentPadding)

	/** テキストが選択されると呼び出される */
	SLATE_EVENT(FOnTextSelectionChanged, OnSelectionChanged)

	/** コンボボックスが開いたときに呼び出される */
	SLATE_EVENT(FOnComboBoxOpening, OnComboBoxOpening)

	/** コンボボックスが選択されたアイテムを確立する必要があるときに呼び出されます */
	SLATE_ARGUMENT(TSharedPtr<FString>, InitiallySelectedItem)

	/** [Optional]  現在選択されているアイテムのラベルを取得するために呼び出される */
	SLATE_EVENT(FGetTextComboLabel, OnGetTextLabelForItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** 各文字列のウィジェットを作成するために呼び出される */
	TSharedRef<SWidget> MakeItemWidget(TSharedPtr<FString> StringItem);

	void SetSelectedItem(TSharedPtr<FString> NewSelection);

	/** 現在選択されている文字列を返す */
	TSharedPtr<FString> GetSelectedItem() {
		return SelectedItem;
	}

	/** コンボボックスのテキストオプションをOptionsSource属性から再読み込みするリクエスト */
	void RefreshOptions();

	/** テキストコンボで選択された項目をクリアする */
	void ClearSelection();

private:
	TSharedPtr<FString> OnGetSelection() const { return SelectedItem; }

	/** コンボのポップアップで選択が変わったときに呼び出される */
	void OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo);

	/** コンボボックス内の指定された項目のテキストを取得するヘルパーメソッド */
	FText GetSelectedTextLabel() const;

	FText GetItemTextLabel(TSharedPtr<FString> StringItem) const;

private:

	/** アイテムのテキストラベルを取得するために呼び出される */
	FGetTextComboLabel GetTextLabelForItem;

	/** 選択されたテキストアイテム */
	TSharedPtr<FString> SelectedItem;

	/** コンボ・ウィジェットが文字列を処理できるようにするための、文字列への共有ポインタの配列 */
	TArray< TSharedPtr<FString> >		Strings;

	/** コンボボックスへのポインタ */
	TSharedPtr<SCustomComboBox> 	StringCombo;

	/** Forwarding Delegate　テキストが変更されたときに呼び出される関数のデリゲート */
	FOnTextSelectionChanged SelectionChanged;

	/** テキストの描画に使用するフォントの設定 */
	TAttribute< FSlateFontInfo > Font;
};