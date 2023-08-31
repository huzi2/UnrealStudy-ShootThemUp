// Shoot Them Up Game. All Rightes Reserved.


#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
	, bWantsToRun(false)
	, bIsMovingForward(false)
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CameraCollisionComponent"));
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	DefaultInputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultInputMappingContext"));
	MoveForwardInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardInputAction"));
	MoveRightInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightInputAction"));
	LookUpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("LookUpInputAction"));
	TurnAroundInputAction = CreateDefaultSubobject<UInputAction>(TEXT("TurnAroundInputAction"));
	JumpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("JumpInputAction"));
	RunInputAction = CreateDefaultSubobject<UInputAction>(TEXT("RunInputAction"));
	FireInputAction = CreateDefaultSubobject<UInputAction>(TEXT("FireInputAction"));
	NextWeaponInputAction = CreateDefaultSubobject<UInputAction>(TEXT("NextWeaponInputAction"));
	ReloadInputAction = CreateDefaultSubobject<UInputAction>(TEXT("ReloadInputAction"));
	ZoomInputAction = CreateDefaultSubobject<UInputAction>(TEXT("ZoomInputAction"));
}

void ASTUPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::MoveForward);
		Input->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::MoveRight);
		Input->BindAction(LookUpInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::LookUp);
		Input->BindAction(TurnAroundInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::TurnAround);
		Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Jump);
		Input->BindAction(RunInputAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::OnStartRunning);
		Input->BindAction(RunInputAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::OnStopRunning);
		Input->BindAction(FireInputAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
		Input->BindAction(FireInputAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);
		Input->BindAction(NextWeaponInputAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);
		Input->BindAction(ReloadInputAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::Reload);
		Input->BindAction(ZoomInputAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::Zoom, true);
		Input->BindAction(ZoomInputAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::Zoom, false);
	}
}

void ASTUPlayerCharacter::OnDeath()
{
	Super::OnDeath();

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

bool ASTUPlayerCharacter::IsRunning() const
{
	return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	bIsMovingForward = MovementValue > 0.f;
	if (MovementValue == 0.f)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector(), MovementValue);
}

void ASTUPlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();
	if (MovementValue == 0.f)
	{
		return;
	}

	AddMovementInput(GetActorRightVector(), MovementValue);
}

void ASTUPlayerCharacter::LookUp(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	AddControllerPitchInput(MovementValue);
}

void ASTUPlayerCharacter::TurnAround(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	AddControllerYawInput(MovementValue);
}

void ASTUPlayerCharacter::OnStartRunning()
{
	bWantsToRun = true;
}

void ASTUPlayerCharacter::OnStopRunning()
{
	bWantsToRun = false;
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
	const bool bHideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(bHideMesh);
	
	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (USceneComponent* MeshChild : MeshChildren)
	{
		UPrimitiveComponent* MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(bHideMesh);
		}
	}
}
