// Fill out your copyright notice in the Description page of Project Settings.


#include "Repulsor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ARepulsor::ARepulsor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARepulsor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ARepulsor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ARepulsor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARepulsor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector LaunchVelocity = GetActorForwardVector();
	FRotator LaunchRot = UKismetMathLibrary::MakeRotFromX(LaunchVelocity);
	LaunchRot.Pitch += 45.f;

	if (OtherActor != GetOwner() && OtherActor != nullptr && OtherComp->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Overlap"));
		OtherComp->AddImpulse(LaunchRot.Vector() * 2000.0f, FName (NAME_None), true);
		
	}

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);

	if (OtherCharacter && OtherCharacter != UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		
		OtherCharacter->LaunchCharacter(LaunchRot.Vector() * 2000.f, true, true);
	}
}



