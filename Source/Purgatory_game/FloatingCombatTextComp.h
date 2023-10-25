// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingCombatTextComp.generated.h"

class AFloatingTextActor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURGATORY_GAME_API UFloatingCombatTextComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFloatingCombatTextComp();

private:
	// floating combat text actor used to display text
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComp", Meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AFloatingTextActor> FloatingTextActorClass = nullptr;

	//amount of vertical space between numbers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComp", Meta = (AllowPrivateAccess = "true"))
		float TextVerticalSpacing = 40.0f;

	//amount of vert offset to apply
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComp", Meta = (AllowPrivateAccess = "true"))
		float TextVericalOffset = 100.0f;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// adds the specified text at specified location in worldspace
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "FloatingCombatTextComponent")
		void AddFloatingText_CLIENT(const FText& text, FVector world_location);

private:
	//callback for when floating text actor is destroyed
	UFUNCTION()
		void OnTextDestroyed(AActor* destroyed_actor);

	//ARRAY OF ALL ACTIVE FLOATING TEXT ACTORS
	UPROPERTY()
		TArray<AFloatingTextActor*> ActiveTextActors;

		
};
