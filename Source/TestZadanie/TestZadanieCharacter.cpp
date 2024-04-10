// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestZadanieCharacter.h"
#include "TestZadanieProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "BuildGameInstance.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ATestZadanieCharacter

ATestZadanieCharacter::ATestZadanieCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

}

void ATestZadanieCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestZadanieCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ATestZadanieCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestZadanieCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATestZadanieCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATestZadanieCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Build1", IE_Pressed, this, &ATestZadanieCharacter::SpawnSourceBuild);
	PlayerInputComponent->BindAction("Build2", IE_Pressed, this, &ATestZadanieCharacter::SpawnTransmitterBuild);
	PlayerInputComponent->BindAction("Build3", IE_Pressed, this, &ATestZadanieCharacter::SpawnPortalBuild);

	PlayerInputComponent->BindAction("PrevLevel", IE_Pressed, this, &ATestZadanieCharacter::PrevLevel);
	PlayerInputComponent->BindAction("NextLevel", IE_Pressed, this, &ATestZadanieCharacter::NextLevel);
}

void ATestZadanieCharacter::SpawnBuild(EBuilding Type)
{
	UWorld* const World = GetWorld();
	if (!World)
		return;
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (!GI)
		return;
	const FVector SpawnVector = GetActorLocation() + (GetActorForwardVector() * 100.f);
	const FRotator SpawnRotation = GetActorRotation();
	SUniqBuild UniqBuild;
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABuildingParent* SpawnActor;
	switch (Type)
	{
	case EBuilding::SourceBuild:
		if (SourceBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(SourceBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
			GI->SavedBuilds.Add(SpawnActor);
		}
		break;

	case EBuilding::TransmitterBuild:
		if (TransmitterBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(TransmitterBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
			GI->SavedBuilds.Add(SpawnActor);
		}
		break;

	case EBuilding::PortalBuild:
		if (PortalBuild != nullptr)
		{
			SpawnActor = World->SpawnActor<ABuildingParent>(PortalBuild, SpawnVector, SpawnRotation, ActorSpawnParams);
			SpawnActor->InitParams();
			SavedBuilds.Add(SpawnActor);
			GI->SavedBuilds.Add(SpawnActor);
		}
		break;
	default:
		break;
	}
}

void ATestZadanieCharacter::SpawnSourceBuild()
{
	SpawnBuild(EBuilding::SourceBuild);
}

void ATestZadanieCharacter::SpawnTransmitterBuild()
{
	SpawnBuild(EBuilding::TransmitterBuild);
}

void ATestZadanieCharacter::SpawnPortalBuild()
{
	SpawnBuild(EBuilding::PortalBuild);
}

void ATestZadanieCharacter::PrevLevel()
{
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->CurrentLevel - 1 >= 0 ? GI->ChangeLevel(GI->CurrentLevel, GI->CurrentLevel - 1) : false;
		for (int i = 0; i < SavedBuilds.Num(); i++)
		{
			SavedBuilds[i]->Destroy();
		}
		SavedBuilds.Empty();
		GI->CheckTargetBuild();
	}
}

void ATestZadanieCharacter::NextLevel()
{
	UBuildGameInstance* GI = Cast<UBuildGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->CurrentLevel + 1 <= 1000 ? GI->ChangeLevel(GI->CurrentLevel, GI->CurrentLevel + 1) : false;
		for (int i = 0; i < SavedBuilds.Num(); i++)
		{
			SavedBuilds[i]->Destroy();
		}
		SavedBuilds.Empty();
		GI->CheckTargetBuild();
	}
}

void ATestZadanieCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATestZadanieCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATestZadanieCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATestZadanieCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
