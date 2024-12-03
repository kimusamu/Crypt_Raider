// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	/*
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr) {
		UE_LOG(LogTemp, Display, TEXT("Got Physics Handle: %s"), *PhysicsHandle->GetName());
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle Found!"));
	}
	*/
	
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr) {
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
		FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

	/*
	FRotator MyRotation = GetComponentRotation();
	FString RotationString = MyRotation.ToCompactString();
	UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);

	float Time = GetWorld()->TimeSeconds;
	UE_LOG(LogTemp, Display, TEXT("Current Time Is: %f"), Time);
	*/

	/*
	float Damage;
	if (HasDamage(Damage)) {
		PrintDamage(Damage);
	}
	// float& DamageRef = Damage;
	// DamageRef = 5;
	// UE_LOG(LogTemp, Display, TEXT("DamageRef: %f, Damage: %f"), DamageRef, Damage);
	// UE_LOG(LogTemp, Display, TEXT("Original Damage: %f"), Damage);
	*/
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle == nullptr) {
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit) {
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}

	/*
	if (HasHit) {
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
	}

	else {
		UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));
	}
	*/
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Display, TEXT("Release Grabber"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr) {
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr) {
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result =  GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (Result == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	
	return Result;
}

/*
void UGrabber::PrintDamage(const float& Damage)
{
	// Damage = 2;
	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);

}

bool UGrabber::HasDamage(float& OutDamage)
{
	OutDamage = 5;
	return true;
}
*/

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);	
}
