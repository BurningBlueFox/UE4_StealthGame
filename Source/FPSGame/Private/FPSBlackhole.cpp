// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackhole.h"

// Sets default values
AFPSBlackhole::AFPSBlackhole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComp->SetSphereRadius(100);
	InnerSphereComp->SetupAttachment(MeshComp);

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackhole::OverlappingInnerSphere);

	OutterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OutterSphereComp"));
	OutterSphereComp->SetSphereRadius(3000);
	OutterSphereComp->SetupAttachment(MeshComp);
}

void AFPSBlackhole::OverlappingInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
										   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	OutterSphereComp->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = OutterSphereComp->GetScaledSphereRadius();
			const float ForceStrenght = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrenght, RIF_Constant, true);
		}
	}

}

