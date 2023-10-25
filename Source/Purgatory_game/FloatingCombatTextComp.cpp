// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCombatTextComp.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "FloatingTextActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UFloatingCombatTextComp::UFloatingCombatTextComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloatingCombatTextComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFloatingCombatTextComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner<APawn>()->IsLocallyControlled() == false)
		return;

	//gets viewport size
	const FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	//checks for bad viewport size
	if (viewportSize.X <= 0.0f || viewportSize.Y <= 0.0f)
		return;

	//gets viewport scale(DPI scale)
	const int32 viewportX = FGenericPlatformMath::FloorToInt(viewportSize.X);
	const int32 viewportY = FGenericPlatformMath::FloorToInt(viewportSize.Y);
	const float viewportscale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportX, viewportY));

	if (viewportscale <= 0.0f)
		return;
	//gets the players camera location
	const FVector cameraLocation = GetOwner<APawn>()->GetController<APlayerController>()->PlayerCameraManager->GetCameraLocation();

	//adjusting the vertical location of the active text actors by the viewport size and scale so that they appear evenly stacked on the screen
	for (int32 i = 1; i < ActiveTextActors.Num(); ++i) {
		//calculating the vertical offset based on how far the camera is from the floating text actor
		AFloatingTextActor* textActor = ActiveTextActors[i];
		const float distance = FVector::Dist(cameraLocation, textActor->GetAnchorLocation());
		const float verticalOffset = distance / (viewportSize.X / viewportscale);

		//adjust the floating text actors location by the calculated amount
		textActor->SetActorLocation(textActor->GetAnchorLocation() + FVector(0.0f, 0.0f, i * verticalOffset * TextVerticalSpacing));
	}
}

void UFloatingCombatTextComp::AddFloatingText_CLIENT_Implementation(const FText& text, FVector world_location)
{
	if (FloatingTextActorClass == nullptr)
		return;
	//spawns the new text actor at the specified location
	const FTransform spawnTransform = FTransform(FRotator::ZeroRotator, world_location + FVector(0.0f, 0.0f, TextVericalOffset));
	AFloatingTextActor* newTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, spawnTransform, GetOwner());
	if (newTextActor == nullptr)
		return;

	//initialize and finish spawning actor
	newTextActor->Initialize(text);
	newTextActor->OnDestroyed.AddDynamic(this, &UFloatingCombatTextComp::OnTextDestroyed);
	UGameplayStatics::FinishSpawningActor(newTextActor, spawnTransform);

	ActiveTextActors.Insert(newTextActor, 0);
}

void UFloatingCombatTextComp::OnTextDestroyed(AActor* destroyed_actor)
{
	ActiveTextActors.Pop();
}


