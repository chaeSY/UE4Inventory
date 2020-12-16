// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "SYDefine.h"
#include "ItemSlotWidgetBase.generated.h"
/**
 * 
 */

struct FItemInfo;

UCLASS()
class GAMEDATA_API UItemSlotWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()

public:
	void SetSlotIndex(int InSlotIndex);
	void SetParentUINumber(EUINumber UINumber);
	void UpdateSlot(const FItemInfo& InItemInfo);

	DECLARE_EVENT_OneParam(UItemSlotWidgetBase, FSlotHoverEvent, int32); // Param1: SlotIndex
	FSlotHoverEvent OnHovered;

private:
	virtual void NativeConstruct() final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) final;
	
	virtual auto CreateSrcDragDropOp() ->UDragDropOperation* final;
	virtual void SetDstDragDropOp(UDragDropOperation* OutDragDropOp) final;
	virtual auto CreateButtonDownOp(FKey key) -> USYMouseButtonDownOp* final;

	void BindWidget();
	bool IsShowItemCount(const FItemInfo& InItemInfo);

private:
	EUINumber	ParentUINumber;
	int			SlotIndex;

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