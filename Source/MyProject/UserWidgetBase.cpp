// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

#if WITH_EDITOR
const FText UUserWidgetBase::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "CustomPaletteCategory", "Me");
}
#endif
