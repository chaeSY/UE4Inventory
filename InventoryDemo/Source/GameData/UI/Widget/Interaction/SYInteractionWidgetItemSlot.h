// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYInteractionWidgetBase.h"
#include "SYDefine.h"
#include "SYWidgetDefine.h"
#include "SYInteractionWidgetItemSlot.generated.h"
/**
 * 
 */

UCLASS()
class UDragDropPayloadSlot : public UDragDropPayloadBase
{
	GENERATED_BODY()

public:
	int SrcSlotIndex;
	int DstSlotIndex;
};

struct FItemInfo;
class UImage;
class UBorder;
class UTextBlock;
class UTexture2D;

UCLASS()
class GAMEDATA_API USYInteractionWidgetItemSlot : public USYInteractionWidgetBase
{
	GENERATED_BODY()

public:
	FSlotEvent& OnMouseOver();
	FSlotEvent& OnMouseRButtonDown();
	FSlotDragDropEvent& OnDragDrop();

	void SetSlotIndex(int InSlotIndex);
	void SetParentUINumber(EUINumber UINumber);
	void UpdateSlot(const FItemInfo& InItemInfo);

private:
	virtual void NativeConstruct() final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) final;
	virtual void OnDragDropInternal(UDragDropOperation* InOperation) final;
	virtual void OnMouseRButtonDownInternal() final;

	virtual auto CreatePayload()->UDragDropPayloadBase*;
	virtual void SetPayloadOnDrop(UDragDropPayloadBase* Payload);

	void BindWidget();
	bool IsShowItemCount(const FItemInfo& InItemInfo);

private:
	int					SlotIndex;
	FSlotEvent			MouseOverEvent;
	FSlotEvent			MouseRButtonDownEvent;
	FSlotDragDropEvent	DragDropEvent;

	UPROPERTY()
	class UImage* ItemIconImage;

	UPROPERTY()
	class UTextBlock* ItemCountText;

	UPROPERTY()
	class UBorder* ItemSlotBorder;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* BackgroundTexture;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* BackgroundTextureMouseOver;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* EmptyItemIconTexture;
};