#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

AMyPawn::AMyPawn()
{
    PrimaryActorTick.bCanEverTick = true;


    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->InitCapsuleSize(34.f, 88.f);
    Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Capsule->SetCollisionObjectType(ECC_Pawn);
    Capsule->SetCollisionResponseToAllChannels(ECR_Block);
    Capsule->SetSimulatePhysics(false);
    RootComponent = Capsule;


    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    Mesh->SetSimulatePhysics(false);


    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 350.f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 10.f;
    SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 60.f));


    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

void AMyPawn::BeginPlay()
{
    Super::BeginPlay();
    if (Mesh->IsSimulatingPhysics())
    {
        Mesh->SetSimulatePhysics(false);
    }

}

void AMyPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput
        = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AMyPlayerController* PlayerController
            = Cast<AMyPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction
                    , ETriggerEvent::Triggered
                    , this
                    , &AMyPawn::Move);
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction
                    , ETriggerEvent::Triggered
                    , this
                    , &AMyPawn::Look);

            }
        }
    }
}
void AMyPawn::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2d MoveInput = value.Get<FVector2d>();

    const float Dt = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.016f;

    const FVector Forward = GetActorForwardVector();
    const FVector Right = GetActorRightVector();
    FVector Delta = (Forward * MoveInput.Y + Right * MoveInput.X);
    Delta.Z = 0.f;

    AddActorWorldOffset(Delta.GetSafeNormal() * MoveSpeed * Dt, true);
}

void AMyPawn::Look(const FInputActionValue& value)
{
    FVector2d LookInput = value.Get<FVector2d>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}