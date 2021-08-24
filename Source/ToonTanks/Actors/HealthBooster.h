// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthBooster.generated.h"
class UCapsuleComponent;
UCLASS()
class TOONTANKS_API AHealthBooster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthBooster();
	UPROPERTY(EditAnywhere)
	int32 HealthPortion = 100;

	private:
	int32 HealedActors = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
