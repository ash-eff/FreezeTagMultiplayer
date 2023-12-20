// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "DeathTag/DeathTagTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "DeathTagCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;

UCLASS()
class DEATHTAG_API ADeathTagCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADeathTagCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* CharacterContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchPressed;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchReleased;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimPressedAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimReleasedAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipButtonPressed(const FInputActionValue& Value);
	void CrouchButtonPressed(const FInputActionValue& Value);
	void CrouchButtonReleased(const FInputActionValue& Value);
	void AimPressed(const FInputActionValue& Value);
	void AimReleased(const FInputActionValue& Value);
	void AimOffset(float DeltaTime);
	virtual void Jump() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	float AO_Yaw;
	float Interp_AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAOYaw() const { return AO_Yaw; }
	FORCEINLINE float GetAOPitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
};
