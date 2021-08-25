// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(TurretMesh);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    
    ForcePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Force apply point"));
    ForcePoint->SetupAttachment(TurretMesh);
    
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());

    HealthComponent = FindComponentByClass<UHealthComponent>();
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();
    // Hide Player. TODO - Create new funtion to handle this.
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive()
{
    return bIsPlayerAlive;
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Rotate();
    Move();

    if(PlayerControllerRef)
    {
        /*FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);*/
        TurretMesh->AddLocalRotation(TurretRotationDirection);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
    PlayerInputComponent->BindAxis("TurretRotationButtons", this, &APawnTank::UpdateTurretRotation);
}

void APawnTank::CalculateMoveInput(float Value) 
{
    MoveDirection = (BoxComp->GetForwardVector()) * Value;

    //DesiredSpeed = Value * MSpeed;
    
}

void APawnTank::CalculateRotateInput(float Value) 
{
    float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotateAmount, 0);
    RotationDirection = FQuat(Rotation);    
}

void APawnTank::Move() 
{
    // AddActorLocalOffset(MoveDirection, true);
    // FVector DownVector = BoxComp->GetUpVector()*(-1);
    // BoxComp->AddForceAtLocation(MoveDirection, ForcePoint->GetComponentLocation());
    // BoxComp->AddImpulseAtLocation(MoveDirection, ForcePoint->GetComponentLocation());
    // FVector DownVector = BoxComp->GetUpVector() * (-1) * 500;
    // BoxComp->AddImpulseAtLocation(DownVector, ForcePoint->GetComponentLocation());
    // BoxComp->SetPhysicsLinearVelocity(FVector(0, 0, 0));
    MoveDirection *= 400;
    UE_LOG(LogTemp, Warning, TEXT(" current speed %f"), GetVelocity().Size());
    UE_LOG(LogTemp, Warning, TEXT(" desired speed %f"), (MoveDirection).Size());
    FVector CurrentVelocity = GetVelocity();
    DrawDebugLine(GetWorld(), ForcePoint->GetComponentLocation(), ForcePoint->GetComponentLocation() + CurrentVelocity,  FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);
    FVector DeltaForce = (MoveDirection - CurrentVelocity) * BoxComp->GetMass() * 2.5; //* GetWorld()->DeltaTimeSeconds;
    DrawDebugLine(GetWorld(), ForcePoint->GetComponentLocation(), ForcePoint->GetComponentLocation() + DeltaForce,  FColor(0, 255, 0), false, 0.0f, 0.0f, 10.0f);
    DrawDebugLine(GetWorld(), ForcePoint->GetComponentLocation(), ForcePoint->GetComponentLocation() + MoveDirection,  FColor(0, 0, 255), false, 0.0f, 0.0f, 10.0f);
    BoxComp->AddForceAtLocation(DeltaForce, ForcePoint->GetComponentLocation());
    BoxComp->AddForceAtLocation(BoxComp->GetUpVector()*(-1)*1000, ForcePoint->GetComponentLocation());

    
    //float DeltaSpeed = DesiredSpeed - GetVelocity().Size();
    //UE_LOG(LogTemp, Warning, TEXT(" delta speed %f"), DeltaSpeed);
    //FVector ForceVector = (BoxComp->GetForwardVector()) * DeltaSpeed * GetWorld()->DeltaTimeSeconds * 4000;
    //UE_LOG(LogTemp, Warning, TEXT(" force %s"), *ForceVector.ToString());
    // BoxComp->AddImpulseAtLocation(ForceVector, ForcePoint->GetComponentLocation());


    
    
}

void APawnTank::Rotate() 
{
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::UpdateTurretRotation(float Value)
{
    float Speed = 70.0f;
    float RotateAmount = Value * Speed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotateAmount, 0);
    TurretRotationDirection = FQuat(Rotation);    
}
