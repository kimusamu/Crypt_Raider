// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();

	if (Actor != nullptr) {
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());

		if (Component != nullptr) {
			Component->SetSimulatePhysics(false);
		}

		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);
		// UE_LOG(LogTemp, Display, TEXT("Unlocking"));
	}

	else {
		Mover->SetShouldMove(false);
		// UE_LOG(LogTemp, Display, TEXT("Relocking"));
	}

	/*
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	
	int32 index = 0;

	while (index < Actors.Num()) {
		FString ActorName = Actors[i]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("%s"), *ActorName);
		++index;
	}
	

	
	for (int32 i = 0; i < Actors.Num(); ++i) {
		FString ActorName = Actors[i]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("%s"), *ActorName);
	}
	
	
	for (AActor* Actor : Actors) {
		// FString ActorName = Actor->GetActorNameOrLabel();
		// UE_LOG(LogTemp, Display, TEXT("%s"), *ActorName);

		if (Actor->ActorHasTag(AcceptableActorTag)) {
			UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		}
	}
	*/
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors) {
		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");

		if (HasAcceptableTag && !IsGrabbed) {
			return Actor;
		}
	}

	return nullptr;
}

