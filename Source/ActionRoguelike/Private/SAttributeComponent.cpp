// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	health = 100;
	// ...
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	health += Delta;
	return true;
}
