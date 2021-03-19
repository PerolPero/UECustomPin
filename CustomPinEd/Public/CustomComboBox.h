#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Layout/Margin.h"
#include "Styling/SlateColor.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Sound/SlateSound.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Framework/SlateDelegates.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Views/STableViewBase.h"
#include "Framework/Views/TableViewTypeTraits.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SSearchBox.h"

DECLARE_DELEGATE(FOnComboBoxOpening)

/**
 * 任意のコンテンツを表示するコンボボックスです
 */
class SCustomComboBox : public SComboButton
{
public:
    /** メニューの選択肢を表示するためのリストの種類 */
    typedef SListView< TSharedPtr<FString> > SComboListType;
    /** オプションを表すウィジェットの生成に使用されるデリゲートタイプ */
    typedef typename TSlateDelegates< TSharedPtr<FString> >::FOnGenerateWidget FOnGenerateWidget;
    typedef typename TSlateDelegates< TSharedPtr<FString> >::FOnSelectionChanged FOnSelectionChanged;
    TSharedPtr<SSearchBox> SearchBox;

    SLATE_BEGIN_ARGS(SCustomComboBox)
        : _Content()
        , _ComboBoxStyle(&FCoreStyle::Get().GetWidgetStyle< FComboBoxStyle >("ComboBox"))
        , _ButtonStyle(nullptr)
        , _ItemStyle(&FCoreStyle::Get().GetWidgetStyle< FTableRowStyle >("TableView.Row"))
        , _ContentPadding(FMargin(4.0, 2.0))
        , _ForegroundColor(FCoreStyle::Get().GetSlateColor("InvertedForeground"))
        , _OptionsSource()
        , _OnSelectionChanged()
        , _OnGenerateWidget()
        , _InitiallySelectedItem(nullptr)
        , _Method()
        , _MaxListHeight(450.0f)
        , _HasDownArrow(true)
        , _EnableGamepadNavigationMode(false)
        , _IsFocusable(true)
    {
    }

    /** このボタンのコンテンツのスロット（オプション） */
    SLATE_DEFAULT_SLOT(FArguments, Content)

    SLATE_STYLE_ARGUMENT(FComboBoxStyle, ComboBoxStyle)

    /** コンボボックスのボタン部分のビジュアルスタイル (overrides ComboBoxStyle) */
    SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)

    SLATE_STYLE_ARGUMENT(FTableRowStyle, ItemStyle)

    SLATE_ATTRIBUTE(FMargin, ContentPadding)
    SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)

    SLATE_ARGUMENT(const TArray< TSharedPtr<FString> >*, OptionsSource)
    SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)
    SLATE_EVENT(FOnGenerateWidget, OnGenerateWidget)
    //SLATE_EVENT(FOnTextChanged, OnSearchTextChanged)

    /** コンボボックスが開かれたとき、リストが実際に作成される前に呼び出される */
    SLATE_EVENT(FOnComboBoxOpening, OnComboBoxOpening)

    /** リストビューで使用するカスタムスクロールバー */
    SLATE_ARGUMENT(TSharedPtr<SScrollBar>, CustomScrollbar)

    /** コンボボックスが最初に作成されたときに選択されるべきオプション */
    SLATE_ARGUMENT(TSharedPtr<FString>, InitiallySelectedItem)

    SLATE_ARGUMENT(TOptional<EPopupMethod>, Method)

    /** コンボボックスメニューの最大の高さ */
    SLATE_ARGUMENT(float, MaxListHeight)

    /** ボタンを押したときに鳴らす音 (overrides ComboBoxStyle) */
    SLATE_ARGUMENT(TOptional<FSlateSound>, PressedSoundOverride)

    /** 選択肢が変わったときに再生する音 (overrides ComboBoxStyle) */
    SLATE_ARGUMENT(TOptional<FSlateSound>, SelectionChangeSoundOverride)

    /**
     * falseの場合、下向きの矢印は生成されず、APIコンシューマーがドロップダウンであることを示す視覚的なヒントを独自に作成することになります。
     */
    SLATE_ARGUMENT(bool, HasDownArrow)

    /**
     *  falseの場合、方向キーで選択範囲が変わります。trueの場合、ComboBoxはアクティブにする必要があります。
     *  アクティブになっている間は矢印入力のみを取り込みます。
     */
    SLATE_ARGUMENT(bool, EnableGamepadNavigationMode)

    /** trueの場合、コンボボックスがキーボードフォーカスを受けることができます。 */
    SLATE_ARGUMENT(bool, IsFocusable)

    SLATE_END_ARGS()

    /**
     * ウィジェットを構築する
     *
     * @param InArgs   コンボボックスを作成するための宣言
     */
    void Construct(const FArguments& InArgs)
    {
        check(InArgs._ComboBoxStyle);

        ItemStyle = InArgs._ItemStyle;

        // オーバーライドされたか、スタイルのバージョンを使うべきかに基づいて、どの値を使うべきかを検討する。
        const FComboButtonStyle& OurComboButtonStyle = InArgs._ComboBoxStyle->ComboButtonStyle;
        const FButtonStyle* const OurButtonStyle = InArgs._ButtonStyle ? InArgs._ButtonStyle : &OurComboButtonStyle.ButtonStyle;
        PressedSound = InArgs._PressedSoundOverride.Get(InArgs._ComboBoxStyle->PressedSlateSound);
        SelectionChangeSound = InArgs._SelectionChangeSoundOverride.Get(InArgs._ComboBoxStyle->SelectionChangeSlateSound);

        this->OnComboBoxOpening = InArgs._OnComboBoxOpening;
        this->OnSelectionChanged = InArgs._OnSelectionChanged;
        this->OnGenerateWidget = InArgs._OnGenerateWidget;
        //this->OnSearchTextChanged = InArgs._OnSearchTextChanged;
        this->EnableGamepadNavigationMode = InArgs._EnableGamepadNavigationMode;
        FilterdArray = new TArray<TSharedPtr<FString>>();
        OptionsSource = InArgs._OptionsSource;
        CustomScrollbar = InArgs._CustomScrollbar;
        FilterdArray->Reset();
        for (int i = 0; i < OptionsSource->Num(); i++)
        {
            FilterdArray->Add(OptionsSource->GetData()[i]);
        }


        TSharedRef<SWidget> ComboBoxMenuContent =
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SAssignNew(SearchBox, SSearchBox)
                .OnTextChanged(this, &SCustomComboBox::OnSearchTextChanged)
            ]
            + SVerticalBox::Slot()
            .HAlign(HAlign_Left)
            [
                SNew(SBox)
                .MaxDesiredHeight(InArgs._MaxListHeight)
                [
                    SAssignNew(this->ComboListView, SComboListType)
                    .ListItemsSource(FilterdArray)
                    .OnGenerateRow(this, &SCustomComboBox::GenerateMenuItemRow)
                    .OnSelectionChanged(this, &SCustomComboBox::OnSelectionChanged_Internal)
                    .SelectionMode(ESelectionMode::Single)
                    .ExternalScrollbar(InArgs._CustomScrollbar)
                ]
            ];

        // コンテンツのセットアップ
        TSharedPtr<SWidget> ButtonContent = InArgs._Content.Widget;
        if (InArgs._Content.Widget == SNullWidget::NullWidget)
        {
            SAssignNew(ButtonContent, STextBlock)
            .Text(NSLOCTEXT("SCustomComboBox", "ContentWarning", "No Content Provided"))
            .ColorAndOpacity(FLinearColor::Red);
        }


        SComboButton::Construct(
            SComboButton::FArguments()
            .ComboButtonStyle(&OurComboButtonStyle)
            .ButtonStyle(OurButtonStyle)
            .Method(InArgs._Method)
            .ButtonContent()
            [
                ButtonContent.ToSharedRef()
            ]
            .MenuContent()
            [
                ComboBoxMenuContent
            ]
            .HasDownArrow(InArgs._HasDownArrow)
            .ContentPadding(InArgs._ContentPadding)
            .ForegroundColor(InArgs._ForegroundColor)
            .OnMenuOpenChanged(this, &SCustomComboBox::OnMenuOpenChanged)
            .IsFocusable(InArgs._IsFocusable)
        );
        SetMenuContentWidgetToFocus(ComboListView);

        // 構築時に選択されたアイテムを確立し、問い合わせに利用できるようにする必要があります。
        // 注意：選択を有効にする必要がある場合は、IntiallySelectedItemを設定するのではなく、SetItemSelectionを使用してください。
        SelectedItem = InArgs._InitiallySelectedItem;
        if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(SelectedItem))
        {
            ComboListView->Private_SetItemSelection(SelectedItem, true);
            ComboListView->RequestScrollIntoView(SelectedItem, 0);
        }

    }

    void ClearSelection()
    {
        ComboListView->ClearSelection();
    }

    void SetSelectedItem(TSharedPtr<FString> InSelectedItem)
    {
        if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(InSelectedItem))
        {
            ComboListView->SetSelection(InSelectedItem);
        }
        else
        {
            ComboListView->ClearSelection();
        }
    }

    /** @return コンボボックスで現在選択されている項目を表示します。 */
    TSharedPtr<FString> GetSelectedItem()
    {
        return SelectedItem;
    }

    /**
     * オプションの更新後にリストのリフレッシュを要求します。
     * 必要に応じて SetSelectedItem を呼び出し、選択されたアイテムを更新します。
     * @see SetSelectedItem
     */
    void RefreshOptions()
    {
        ComboListView->RequestListRefresh();
    }

protected:
    /** SListViewが無視するキー押下を処理する */
    virtual FReply OnHandleKeyPressed(FKey KeyPressed)
    {
        if (KeyPressed == EKeys::Enter || KeyPressed == EKeys::SpaceBar || KeyPressed == EKeys::Virtual_Accept)
        {
            TArray<TSharedPtr<FString>> SelectedItems = ComboListView->GetSelectedItems();
            if (SelectedItems.Num() > 0)
            {
                ComboListView->SetSelection(SelectedItems[0]);
            }
            return FReply::Handled();
        }
        else if (KeyPressed == EKeys::Escape)
        {
            this->SetIsOpen(false);
            return FReply::Handled();
        }
        return FReply::Unhandled();
    }

    FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override
    {

        const FKey KeyPressed = InKeyEvent.GetKey();

        if (IsInteractable())
        {

            if (EnableGamepadNavigationMode)
            {
                // コンボボックスの値の操作を開始するには、コントローラの下面ボタンを一度押す必要があります。
                // ボタンが再度押されるか、フォーカスが失われるまで、ウィジェットからのナビゲーションは防止されます。
                if (KeyPressed == EKeys::Enter || KeyPressed == EKeys::SpaceBar || KeyPressed == EKeys::Virtual_Accept)
                {
                    if (bControllerInputCaptured == false)
                    {
                        // コントローラーの入力の取り込みを開始し、ListViewを開く
                        bControllerInputCaptured = true;
                        PlayPressedSound();
                        OnComboBoxOpening.ExecuteIfBound();
                        return SComboButton::OnButtonClicked();
                    }
                    else
                    {
                        // 選択肢をリストの選択された項目に設定して閉じます。
                        bControllerInputCaptured = false;

                        // 最初に選択されたアイテムを再選択する（以前にナビゲーションで選択されていた場合に限る
                        TArray<TSharedPtr<FString>> SelectedItems = ComboListView->GetSelectedItems();
                        if (SelectedItems.Num() > 0)
                        {
                            OnSelectionChanged_Internal(SelectedItems[0], ESelectInfo::Direct);
                        }

                        // コンボボックスにフォーカスを戻す
                        FReply Reply = FReply::Handled();
                        Reply.SetUserFocus(this->AsShared(), EFocusCause::SetDirectly);
                        return Reply;
                    }

                }
                else if (KeyPressed == EKeys::Escape || KeyPressed == EKeys::Virtual_Back || KeyPressed == EKeys::BackSpace)
                {
                    OnMenuOpenChanged(false);
                }
                else
                {
                    if (bControllerInputCaptured)
                    {
                        return FReply::Handled();
                    }
                }
            }
            else
            {
                if (KeyPressed == EKeys::Up || KeyPressed == EKeys::Gamepad_DPad_Up || KeyPressed == EKeys::Gamepad_LeftStick_Up)
                {
                    const int32 SelectionIndex = OptionsSource->Find(GetSelectedItem());
                    if (SelectionIndex >= 1)
                    {
                        // 前の行のアイテムを選択する
                        SetSelectedItem((*OptionsSource)[SelectionIndex - 1]);
                    }

                    return FReply::Handled();
                }
                else if (KeyPressed == EKeys::Down || KeyPressed == EKeys::Gamepad_DPad_Down || KeyPressed == EKeys::Gamepad_LeftStick_Down)
                {
                    const int32 SelectionIndex = OptionsSource->Find(GetSelectedItem());
                    if (SelectionIndex < OptionsSource->Num() - 1)
                    {
                        // 次の行のアイテムを選択する
                        SetSelectedItem((*OptionsSource)[SelectionIndex + 1]);
                    }
                    return FReply::Handled();
                }

                return SComboButton::OnKeyDown(MyGeometry, InKeyEvent);
            }
        }
        return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
    }

    virtual bool SupportsKeyboardFocus() const override
    {
        return bIsFocusable;
    }

    virtual bool IsInteractable() const
    {
        return IsEnabled();
    }

private:

    /** コンボボックスのリスト（OwnerTableとして渡される）のInItemのための行を生成します。これは、ユーザーが指定した OnGenerateWidget を呼び出して行います。 */
    TSharedRef<ITableRow> GenerateMenuItemRow(TSharedPtr<FString> InItem, const TSharedRef<STableViewBase>& OwnerTable)
    {
        if (OnGenerateWidget.IsBound())
        {
            return SNew(SComboRow<TSharedPtr<FString>>, OwnerTable)
                .Style(ItemStyle)
                [
                    OnGenerateWidget.Execute(InItem)
                ];
        }
        else
        {
            return SNew(SComboRow<TSharedPtr<FString>>, OwnerTable)
                [
                    SNew(STextBlock).Text(NSLOCTEXT("SlateCore", "ComboBoxMissingOnGenerateWidgetMethod", "Please provide a .OnGenerateWidget() handler."))
                ];

        }
    }

    //** メニューが閉じられたときに呼び出される
    void OnMenuOpenChanged(bool bOpen)
    {
        if (bOpen == false)
        {
            bControllerInputCaptured = false;

            if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(SelectedItem))
            {
                // ListViewの選択が、最後に選択した選択肢になるようにする
                ComboListView->SetSelection(SelectedItem, ESelectInfo::OnNavigation);
                ComboListView->RequestScrollIntoView(SelectedItem, 0);
                //mboListView->get
            }

            // Set focus back to ComboBox for users focusing the ListView that just closed
            FSlateApplication::Get().ForEachUser([&](FSlateUser* User)
            {
                if (FSlateApplication::Get().HasUserFocusedDescendants(AsShared(), User->GetUserIndex()))
                {
                    FSlateApplication::Get().SetUserFocus(User->GetUserIndex(), AsShared(), EFocusCause::SetDirectly);
                }
            });

        }
    }


    /** リストの選択範囲が変わったときに呼び出される */
    void OnSelectionChanged_Internal(TSharedPtr<FString> ProposedSelection, ESelectInfo::Type SelectInfo)
    {
        // 提案された選択が異なっていることを確認する
        if (SelectInfo != ESelectInfo::OnNavigation)
        {
            // 提案された選択が、選択されたものと異なることを確認する
            if (ProposedSelection != SelectedItem)
            {
                PlaySelectionChangeSound();
                SelectedItem = ProposedSelection;
                OnSelectionChanged.ExecuteIfBound(ProposedSelection, SelectInfo);
            }
            // ユーザーがアイテムを再選択した場合でも閉じる
            this->SetIsOpen(false);
        }
    }

    /** コンテンツメニューをクリックしたときの処理 */
    virtual FReply OnButtonClicked() override
    {
        // ユーザーがクリックしてコンボメニューを閉じた場合
        if (this->IsOpen())
        {
            // 最初に選択されたアイテムを再選択する（以前にナビゲーションで選択されていた場合に限る
            TArray<TSharedPtr<FString>> SelectedItems = ComboListView->GetSelectedItems();
            if (SelectedItems.Num() > 0)
            {
                OnSelectionChanged_Internal(SelectedItems[0], ESelectInfo::Direct);
            }
        }
        else
        {
            PlayPressedSound();
            OnComboBoxOpening.ExecuteIfBound();
        }
        SearchBox->SetText(FText::GetEmpty());
        return SComboButton::OnButtonClicked();
    }

    /** 押した音を再生する */
    void PlayPressedSound() const
    {
        FSlateApplication::Get().PlaySound(PressedSound);
    }

    /** 選択肢を変更した音を再生する */
    void PlaySelectionChangeSound() const
    {
        FSlateApplication::Get().PlaySound(SelectionChangeSound);
    }

    void OnSearchTextChanged(const FText& NewText)
    {
        FilterString = NewText.ToString();
        FilterdArray->Reset();
        if (FilterString.IsEmpty())
        {
            for (int i = 0; i < OptionsSource->Num(); i++)
            {
                FilterdArray->Add(OptionsSource->GetData()[i]);
            }
        }
        else
        {
            FilterdArray->Add(OptionsSource->GetData()[0]);
            for (int i = 1; i < OptionsSource->Num(); i++)
            {
                auto s = OptionsSource->GetData()[i];
                if (s->Contains(FilterString) || s == SelectedItem)
                {
                    FilterdArray->AddUnique(s);
                }
            }
        }
        ComboListView->RebuildList();
    }

    FString FilterString;

    /** ボタンを押したときに再生される音 */
    FSlateSound PressedSound;

    /** 選択肢を変更したときに再生する音 */
    FSlateSound SelectionChangeSound;

    /** 使用するアイテムのスタイル */
    const FTableRowStyle* ItemStyle;

    TArray< TSharedPtr<FString> >* FilterdArray = nullptr;

private:
    /** コンボボックスの選択項目が変更されたときに呼び出されるデリゲート */
    FOnSelectionChanged OnSelectionChanged;
    /** コンボボックスで現在選択されている項目 */
    TSharedPtr<FString> SelectedItem;
    /** ポップアップされたリストビューは、利用可能なオプションを視覚化しています */
    TSharedPtr< SComboListType > ComboListView;
    /** ListViewで使用されるスクロールバーです */
    TSharedPtr< SScrollBar > CustomScrollbar;
    /** コンボボックスが開く前に呼び出されるデリゲート */
    FOnComboBoxOpening OnComboBoxOpening;
    /** オプションをウィジェットとして視覚化する必要があるときに呼び出されるデリゲートです */
    FOnGenerateWidget OnGenerateWidget;
    // リストビューを有効にしたときにトグルするためのアクティベートボタンの使用
    bool EnableGamepadNavigationMode;
    // コントローラ/キーボードがコンボボックスの値を操作しているかどうかを示すフラグを保持します。
    // trueの場合、新しい値が受け入れられるか、キャンセルされるまで、ウィジェットからのナビゲーションが禁止されます。
    bool bControllerInputCaptured;

    const TArray< TSharedPtr<FString> >* OptionsSource;
};