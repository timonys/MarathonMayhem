

#include "PickupDisplayer.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "BaseCharacter.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

// Sets default values
APickupDisplayer::APickupDisplayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Dimensions = FVector(90, 0, 0);
	m_AxisVector = FVector(0, 0, 1);
	m_Multiplier = 180.0f;

	m_PickupModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupDisplayerModel"));
	RootComponent = Cast<USceneComponent>(m_PickupModel);
	m_PickupModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void APickupDisplayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupDisplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (m_Target == nullptr) return;
	
	//Update and clamp angle
	FVector newLocation = m_Target->GetActorLocation();
	m_AngleAxis += DeltaTime * m_Multiplier;

	if (m_AngleAxis >= 360.0f)
	{
		m_AngleAxis = 0.0f;
	}

	//Add rotation to the pickup around the players center point.
	FVector rotateValue = m_Dimensions.RotateAngleAxis(m_AngleAxis, m_AxisVector);
	newLocation.X += rotateValue.X;
	newLocation.Y += rotateValue.Y;
	newLocation.Z += rotateValue.Z;
	
	FRotator newRotation = FRotator(0, m_AngleAxis, 0);
	FQuat quatRotation = FQuat(newRotation);
	SetActorLocationAndRotation(newLocation, quatRotation, false, 0, ETeleportType::None);
	AddActorLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
	if (!m_PickupModel) Destroy();
}

void APickupDisplayer::SetTarget(ABaseCharacter* target)
{
	m_Target = target;

}

void APickupDisplayer::SetPickupModel(UStaticMeshComponent* meshComponent)
{
	
	m_PickupModel->SetStaticMesh(meshComponent->GetStaticMesh());
	m_PickupModel->SetMobility(EComponentMobility::Movable);
	
}