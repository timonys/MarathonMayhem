// Fill out your copyright notice in the Description page of Project Settings.

#include "BoulderSpawner.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "HazardousBoulder.h"
#include "Runtime/Engine/Public/TimerManager.h"

ATargetPoint* ABoulderSpawner::GetRandomSpawnPoint()
{
	int index = FMath::RandRange(0, m_SpawnPoints.Num() - 1);
	return m_SpawnPoints[index];
}

void ABoulderSpawner::SpawnBoulder()
{

	ATargetPoint* spawnPoint = GetRandomSpawnPoint();
	GetWorld()->SpawnActor<AHazardousBoulder>(boulderObject,spawnPoint->GetActorLocation() ,spawnPoint->GetActorRotation());
	--m_BouldersLeft;
}

void ABoulderSpawner::HandleBoulderSpawn(float DeltaTime)
{
	if (m_CanSpawn == false) return;
	
	//spawn a new boulder if the limit has not been reached
	if (m_BouldersLeft > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,FString::SanitizeFloat(m_BouldersLeft) );
		if (m_SpawnTimer >= m_SpawnTime)
		{
			SpawnBoulder();
			m_SpawnTimer = 0.0f;
		}
		else
		{
			m_SpawnTimer += DeltaTime;
		}
	}
	else
	{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Deactivating");
		m_IsActive = false;
		m_SpawnTimer = 0.0f;
		m_CanSpawn = false;
		return;
	}

}

void ABoulderSpawner::Activate()
{
	m_IsActive = true;
	m_BouldersLeft = m_MaxBoulders;
	m_SpawnerCooldown = 10.0f;
}

// Sets default values
ABoulderSpawner::ABoulderSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoulderSpawner::BeginPlay()
{
	Super::BeginPlay();
	m_BouldersLeft = m_MaxBoulders;
}

// Called every frame
void ABoulderSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_SpawnPoints.Num() == 0) { return; }
		
	if (m_IsActive)
	{
		HandleBoulderSpawn(DeltaTime);
	}
	else
	{
		m_SpawnerCooldown -= DeltaTime;
		if (m_SpawnerCooldown <= 0.0f)
		{

			Activate();
		}
	}
}

void ABoulderSpawner::StartSpawning()
{
	m_CanSpawn = true;
}
