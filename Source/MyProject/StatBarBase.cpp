// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"


void UStatBarBase::OnIntStatUpdated(int32 OldValue, int32 NewValue, int32 MaxValue)
{
	OnFloatStatUpdated(static_cast<float>(OldValue), static_cast<float>(NewValue), static_cast<float>(MaxValue));
}

void UStatBarBase::OnFloatStatUpdated(float OldValue, float NewValue, float MaxValue)
{
	if (MaxValue == 0.f) MaxValue = KINDA_SMALL_NUMBER;

	CurrentPercentage = FMath::Clamp(NewValue / MaxValue, 0.f, 1.f);
	CurrentValue = NewValue;
	UpdateWidget();
}

#if WITH_EDITOR
void UStatBarBase::OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs)
{
	Super::OnDesignerChanged(EventArgs);
	UpdateWidget();
}

void UStatBarBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FString PropertyName = ((PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None).ToString();

	if (PropertyName == TEXT("CurrentPercentage") || PropertyName == TEXT("CurrentValue"))
	{
		UpdateWidget();
	}
}
#endif

void UStatBarBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UpdateWidget();
}

void UStatBarBase::ProcessCurrentValueText()
{
	FString FloatString;

	if (CurrentValue < 1000.f)
	{
		FloatString = FString::SanitizeFloat(CurrentValue);

		if (CurrentValue < 100.f)
		{
			int32 StringLen = FloatString.Len();
			if (StringLen > 4)
				FloatString = FloatString.Left(4);
			else if (StringLen < 4)
				FloatString = FloatString.Append("0", 4 - StringLen);
		}
		else
		{
			FloatString = FloatString.Left(3);
		}
	}
	else
	{
		// scaled value
		float ScaledValue = CurrentValue / 1000.f;
		FloatString = FString::SanitizeFloat(ScaledValue);
		if (ScaledValue < 10.f)
			FloatString = FloatString.Left(3).Append(TEXT("k"));
		else
		{
			FloatString = FloatString.Left(2).Append(TEXT("k"));
		}
	}

	CurrentValueText = FText::FromString(FloatString);
}

void UStatBarBase::UpdateWidget()
{
	if (!PercentBar_Filled || !PercentBar_Empty) return;

	FSlateChildSize EmptySize = FSlateChildSize(ESlateSizeRule::Fill);
	EmptySize.Value = 1.f - CurrentPercentage;

	FSlateChildSize FilledSize = FSlateChildSize(ESlateSizeRule::Fill);
	FilledSize.Value = CurrentPercentage;

	if (UHorizontalBoxSlot* FilledSlot = Cast<UHorizontalBoxSlot>(PercentBar_Filled->Slot))
		FilledSlot->SetSize(FilledSize);

	if (UHorizontalBoxSlot* EmptySlot = Cast<UHorizontalBoxSlot>(PercentBar_Empty->Slot))
		EmptySlot->SetSize(EmptySize);

	MainBorder->SetBrushColor(BarBackGroundColor);
	PercentBar_Filled->SetBrushColor(BarForeGroundColor);
	
	ProcessCurrentValueText();

	ValueText->SetText(CurrentValueText);

	PercentBars->SetVisibility(IsFullSize ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
