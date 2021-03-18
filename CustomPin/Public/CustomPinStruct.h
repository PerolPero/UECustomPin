// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomPinStruct.generated.h"

/**
* 
*/
USTRUCT(BlueprintType)
struct CUSTOMPIN_API FCustomPinStruct
{
	GENERATED_BODY()

public:
	FCustomPinStruct(FName InKey);
	FCustomPinStruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key;
};