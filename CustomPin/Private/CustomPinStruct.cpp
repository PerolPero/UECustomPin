// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPinStruct.h"

FCustomPinStruct::FCustomPinStruct(FName InKey)
    :Key(InKey)
{

}

FCustomPinStruct::FCustomPinStruct()
	: Key(_TEXT("None"))
{
}
