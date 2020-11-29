// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ItemDefine.h"
#include "WidgetManager.h"
#include "ItemSlotWidgetBase.generated.h"
/**
 * 
 */

UCLASS()
class GAMEDATA_API UItemSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlotIndex(int InSlotIndex);
	void SetParentUINumber(EUINumber UINumber);
	void UpdateSlot(const FItemInfo& InItemInfo);

	DECLARE_EVENT_OneParam(UItemSlotWidgetBase, FSlotButtonHoverEvent, int32);
	FSlotButtonHoverEvent OnHovered;

private:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	void BindWidget();
	void OnMouseButtonDown(const FPointerEvent& InMouseEvent);
	bool IsShowItemCount(const FItemInfo& InItemInfo);

private:
	EUINumber	ParentUINumber;
	int			SlotIndex;

	UPROPERTY()
	class UImage* ItemSlotIcon;

	UPROPERTY()
	class UTextBlock* ItemCountText;

	UPROPERTY()
	class UBorder* ItemSlotBorder;

	UPROPERTY()
	class UTexture2D* ItemSlotTexture;

	UPROPERTY()
	class UTexture2D* ItemSlotTexture_MouseOver;

	UPROPERTY()
	class UTexture2D* EmptySlotTexture;
};