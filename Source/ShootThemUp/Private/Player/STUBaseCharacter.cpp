// Shoot Them Up Game. All Rightes Reserved.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, bWantsToRun(false)
	, bIsMovingForward(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DefaultInputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultInputMappingContext"));
	MoveForwardInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardInputAction"));
	MoveRightInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightInputAction"));
	LookUpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("LookUpInputAction"));
	TurnAroundInputAction = CreateDefaultSubobject<UInputAction>(TEXT("TurnAroundInputAction"));
	JumpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("JumpInputAction"));
	RunInputAction = CreateDefaultSubobject<UInputAction>(TEXT("RunInputAction"));
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::MoveForward);
		Input->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::MoveRight);
		Input->BindAction(LookUpInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::LookUp);
		Input->BindAction(TurnAroundInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::TurnAround);
		Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Jump);
		Input->BindAction(RunInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnStartRunning);
		Input->BindAction(RunInputAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::OnStopRunning);
	}
}

bool ASTUBaseCharacter::IsRunning() const
{
	return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0.f;
	}

	const FVector VelocityNormal = GetVelocity().GetSafeNormal();
	const float AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const float Degree = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForward(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	bIsMovingForward = MovementValue > 0.f;
	if (MovementValue == 0.f)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector(), MovementValue);
}

void ASTUBaseCharacter::MoveRight(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();
	if (MovementValue == 0.f)
	{
		return;
	}

	AddMovementInput(GetActorRightVector(), MovementValue);
}

void ASTUBaseCharacter::LookUp(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	AddControllerPitchInput(MovementValue);
}

void ASTUBaseCharacter::TurnAround(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	AddControllerYawInput(MovementValue);
}

void ASTUBaseCharacter::OnStartRunning()
{
	bWantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
	bWantsToRun = false;
}
