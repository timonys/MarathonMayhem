#include "PickupBase.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "BaseCharacter.h"


// Sets default values
APickupBase::APickupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	//m_PickupMesh->SetWScale3D(FVector(0.1f));
	RootComponent = Cast<USceneComponent>(m_PickupMesh);

	//Initializing the pickup trigger
	m_pSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	m_pSphereComponent->InitSphereRadius(40.0f);
	m_pSphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	m_pSphereComponent->SetupAttachment(RootComponent);
	m_pSphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_pSphereComponent->SetGenerateOverlapEvents(true);

	m_Rotator = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotator"));
	m_Rotator->RotationRate = FRotator(0.0f, 0.0f, 0.0f);



	m_pSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);
	m_Owner = nullptr;
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool APickupBase::PickedUp()
{
	return m_PickedUp;
}

void APickupBase::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor))
		{

			//pickup added to player and removed from the scene. Player has ownership now
			if (character->IsDead()) return;
			m_Owner = character;
			character->FindComponentByClass<UPickupHolder>()->Clear();
			character->FindComponentByClass<UPickupHolder>()->InsertPickup(this);
			m_PickedUp = true;
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(true);
		}
	}
}

UStaticMeshComponent* APickupBase::GetMesh()const
{
	return m_PickupMesh;
}