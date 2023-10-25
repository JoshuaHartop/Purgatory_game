// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"

UCLASS(Abstract)
class PURGATORY_GAME_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingTextActor();

	//initializes the actor with the required starting info
	UFUNCTION(BlueprintImplementableEvent, Category = "FloatingTextActor")
	void Initialize(const FText& text);

	// gets anchor location
	inline const FVector& GetAnchorLocation() const { return AnchorLocation; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	private:
		// the location to where the text is anchored to
	FVector AnchorLocation;

};
