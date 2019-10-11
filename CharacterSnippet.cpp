void ABaseCharacter::SetIsDead(bool value)
{
	//Changing to a ghost and spawning in middle of the road, to avoid falling out of map
	if (m_IsDead != true)
	{
		m_IsDead = value;
		ClearPickup();
		m_TriggerCapsule->SetGenerateOverlapEvents(true);
		m_DeathSound->Play();
		CallDeathEffects();
		this->GetCharacterMovement()->MaxWalkSpeed = m_MaxGhostSpeed;
		auto controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			FVector spawnLoc;
			
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			FHitResult hit;
			controller->GetHitResultAtScreenPosition(ViewportSize / 2.0f, ECollisionChannel::ECC_Camera, true, hit);
			spawnLoc = hit.Location;
			spawnLoc.Z += 200.0f;
			SetActorLocation(spawnLoc);
		}

		GetMesh()->SetCollisionProfileName(TEXT("GhostCollisionProfile"));
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("GhostCollisionProfile"));
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);;
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

		m_RadialForce->ImpulseStrength = 0.0f;
		if (m_pGhostMaterial && m_GhostMesh)
		{
			//Change the players visuals to ghost visuals
			this->GetMesh()->bPauseAnims = true;
			this->GetMesh()->SetAnimInstanceClass(nullptr);
			this->GetMesh()->SetMaterial(0, m_GhostMI);
			this->GetMesh()->SetSkeletalMesh(m_GhostMesh);

		}

		CallChangeAnimation(true);

	}
	else if (value == false)
	{
		m_IsDead = false;
		m_TriggerCapsule->SetGenerateOverlapEvents(false);
		if (m_HamsterMaterial && m_HamsterMesh)
		{
			//this->GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
			this->GetMesh()->bPauseAnims = true;
			this->GetMesh()->SetAnimInstanceClass(nullptr);
			this->GetMesh()->SetSkeletalMesh(m_HamsterMesh);
			this->GetMesh()->SetMaterial(0, m_HamsterMaterial);
		}
		CallChangeAnimation(false);
	}


}
