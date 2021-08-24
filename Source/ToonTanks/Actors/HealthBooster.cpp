// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBooster.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Componets/HealthComponent.h"

// Sets default values
AHealthBooster::AHealthBooster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHealthBooster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealthBooster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotator(0, 200 * DeltaTime , 0);
	FQuat NewQuat(NewRotator);
	AddActorLocalRotation(NewRotator, false, 0, ETeleportType::None);
	
	if(HealedActors == 0)
	{
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		UE_LOG(LogTemp, Warning, TEXT("Number of overlapping actors: %i"), OverlappingActors.Num());
		for(AActor* CurrentActor: OverlappingActors)
		{
			UHealthComponent* CurrentHealthComponent = CurrentActor->FindComponentByClass<UHealthComponent>();
			if(CurrentHealthComponent && HealedActors == 0)
			{
				CurrentHealthComponent->AddHealth(HealthPortion);
				HealedActors++;
				SetActorHiddenInGame(true);
				SetActorEnableCollision(false);
				SetActorTickEnabled(false);
			}
		}
	}
}

