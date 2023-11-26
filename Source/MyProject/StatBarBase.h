// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "StatBarBase.generated.h"

class UHorizontalBox;
class UBorder;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UStatBarBase : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnIntStatUpdated(int32 OldValue, int32 NewValue, int32 MaxValue);

	UFUNCTION()
	void OnFloatStatUpdated(float OldValue, float NewValue, float MaxValue);

#if WITH_EDITOR
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, Category="Controls", meta = (BindWidget))
	UBorder* MainBorder = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controls", meta = (BindWidget))
	UHorizontalBox* PercentBars = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controls", meta = (BindWidget))
	UBorder* PercentBar_Empty = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controls", meta = (BindWidget))
	UBorder* PercentBar_Filled = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Controls", meta = (BindWidget))
	UTextBlock* ValueText = nullptr;

private:
	UPROPERTY(EditAnywhere, Category="Status Bar")
	FLinearColor BarBackGroundColor = FLinearColor(0.3f, 0.f, 0.f, 0.3f);

	UPROPERTY(EditAnywhere, Category = "Status Bar")
	FLinearColor BarForeGroundColor = FLinearColor(1.f, 0.f, 0.f, 0.75f);

	UPROPERTY(EditAnywhere, Category = "Status Bar")
	bool IsFullSize = true;

	UPROPERTY(EditAnywhere, Category = "Stat Bar | Testing", meta=(ClampMin=0, UIMin=0, ClampMax = 1, Units="Percent"))
	float CurrentPercentage = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Stat Bar | Testing", meta = (ClampMin = 0, UIMin = 0))
	float CurrentValue = 100.f;

	FText CurrentValueText;

	void ProcessCurrentValueText();

	void UpdateWidget();
};
