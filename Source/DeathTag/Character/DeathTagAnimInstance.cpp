// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathTagAnimInstance.h"
#include "DeathTagCharacter.h"
#include "DeathTag/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UDeathTagAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	DeathTagCharacter = Cast<ADeathTagCharacter>(TryGetPawnOwner());
}

void UDeathTagAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(DeathTagCharacter == nullptr)
	{
		DeathTagCharacter = Cast<ADeathTagCharacter>(TryGetPawnOwner());
	}
	if(DeathTagCharacter == nullptr) return;

	FVector Velocity = DeathTagCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = DeathTagCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = DeathTagCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = DeathTagCharacter->IsWeaponEquipped();
	EquippedWeapon = DeathTagCharacter->GetEquippedWeapon();
	bIsCrouched = DeathTagCharacter->bIsCrouched;
	bAiming = DeathTagCharacter->IsAiming();
	TurningInPlace = DeathTagCharacter->GetTurningInPlace();

	// Offset Yaw For Strafing
	FRotator AimRotation = DeathTagCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(DeathTagCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = DeathTagCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = DeathTagCharacter->GetAOYaw();
	AO_Pitch = DeathTagCharacter->GetAOPitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && DeathTagCharacter->GetMesh())
	{
		RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("RightHandSocket"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		DeathTagCharacter->GetMesh()->TransformToBoneSpace(FName("hand_l"), RightHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		RightHandTransform.SetLocation(OutPosition);
		RightHandTransform.SetRotation(FQuat(OutRotation));
	}
}
