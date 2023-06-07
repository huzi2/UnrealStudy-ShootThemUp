// Shoot Them Up Game. All Rightes Reserved.


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, LandedDamageVelocity(FVector2D(900.f, 1200.f))
	, LandedDamage(FVector2D(10.f, 100.f))
	, bWantsToRun(false)
	, bIsMovingForward(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));

	DefaultInputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultInputMappingContext"));
	MoveForwardInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardInputAction"));
	MoveRightInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightInputAction"));
	LookUpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("LookUpInputAction"));
	TurnAroundInputAction = CreateDefaultSubobject<UInputAction>(TEXT("TurnAroundInputAction"));
	JumpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("JumpInputAction"));
	RunInputAction = CreateDefaultSubobject<UInputAction>(TEXT("RunInputAction"));
	FireInputAction = CreateDefaultSubobject<UInputAction>(TEXT("FireInputAction"));
	NextWeaponInputAction = CreateDefaultSubobject<UInputAction>(TEXT("NextWeaponInputAction"));
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

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

	check(PlayerInputComponent);
	check(WeaponComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::MoveForward);
		Input->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::MoveRight);
		Input->BindAction(LookUpInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::LookUp);
		Input->BindAction(TurnAroundInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::TurnAround);
		Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Jump);
		Input->BindAction(RunInputAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::OnStartRunning);
		Input->BindAction(RunInputAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::OnStopRunning);
		Input->BindAction(FireInputAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
		Input->BindAction(FireInputAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);
		Input->BindAction(NextWeaponInputAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);
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

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const float FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

	UE_LOG(LogBaseCharacter, Display, TEXT("On landed : %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X)
	{
		return;
	}

	const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT("Final Damage : %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent(), nullptr, nullptr);
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

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());

	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
