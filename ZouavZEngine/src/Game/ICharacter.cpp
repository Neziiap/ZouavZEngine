#include "Rendering/Material.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/FontComponent.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/RigidBody.hpp"
#include "Component/AudioBroadcaster.hpp"
#include "Component/Animation.hpp"

#include "System/TimeManager.hpp"

#include "GameObject.hpp"
#include "imgui.h"

#include "Game/Arrow.hpp"
#include "Game/Enemy.hpp"
#include "Game/Player.hpp"
#include "Game/ICharacter.hpp"

ICharacter::ICharacter(GameObject* _gameobject, std::string _name)
	: ScriptComponent(_gameobject, _name)
{
}

void ICharacter::OnAddComponent()
{
	GetGameObject().AddComponent<BoxCollision>();
	rb = GetGameObject().AddComponent<RigidBody>();
	rb->lockX = true;
	rb->lockY = true;
	rb->lockZ = true;
	rb->LockAxis();
	material = &GetGameObject().AddComponent<MeshRenderer>()->material;
	attackCollision = GetGameObject().AddComponent<BoxCollision>();
	attackCollision->SetName("Attack Collision");
	attackCollision->SetTrigger(true);
	attackCollision->Deactivate();
	baseColor = material->color;
	GetGameObject().AddComponent<FontComponent>();
	audioBroadcaster = GetGameObject().AddComponent<AudioBroadcaster>();
	animation = GetGameObject().AddComponent<Animation>();
}

void ICharacter::OnTrigger(Object* _other, ShapeCollision* _triggerShape)
{
	GameObject* go = dynamic_cast<GameObject*>(_other);
	if (!go || go->GetComponent<Arrow>())
		return;
	if (_triggerShape == attackCollision)
	{
		Enemy* otherEnemy = go->GetComponent<Enemy>();
		Enemy* thisEnemy = GetGameObject().GetComponent<Enemy>();
		if ((otherEnemy && !thisEnemy) || (!otherEnemy && thisEnemy))
		{
			if (go->GetComponent<ICharacter>()->IsAlive())
			{
				Player* goPlayer = go->GetComponent<Player>();
				if (goPlayer && goPlayer->IsDashing())
					return;
				go->GetComponent<ICharacter>()->Damage(attackDamage);
				if (otherEnemy && !otherEnemy->IsAlive())
					GetGameObject().GetComponent<Player>()->ManageXp(*otherEnemy);
			}
		}
	}
}

void ICharacter::Editor()
{
	ImGui::ColorEdit4("Damage Color : ", &damageColor.x);
	ImGui::DragInt("Life : ", &life, 1.0f, 0, maxLife);
	ImGui::DragInt("Max Life : ", &maxLife, 1.0f, 0, 1000000);
	ImGui::DragFloat("Time before attack : ", &beforeAttack, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Attack Cooldown", &attackCooldown, 0.1f, 0.f);
	ImGui::DragFloat("Invulnerability Fram", &invulnerabilityFrame, 0.1f, 0.f);
	ImGui::DragFloat("Attack Duration", &attackDuration, 0.1f, 0.f);
}

void ICharacter::Begin()
{
	if (GetGameObject().GetComponent<MeshRenderer>())
	{
		material = &GetGameObject().GetComponent<MeshRenderer>()->material;
		baseColor = material->color;
	}
	if (GetGameObject().GetComponent<FontComponent>())
	{
		lifeFont = GetGameObject().GetComponent<FontComponent>();
		lifeFont->ChangeText(std::to_string(life));
	}

	attackCollision = GetGameObject().GetComponentByName<BoxCollision>("Attack Collision");
	rb = GetGameObject().GetComponent<RigidBody>();
	audioBroadcaster = GetGameObject().GetComponent<AudioBroadcaster>();
	animation = GetGameObject().GetComponent<Animation>();
	life > maxLife ? life = maxLife : 0;
	if (animation)
		animation->Play(idleAnimName);
	timerAttackCooldown = attackCooldown;
	timerAttackDuration = attackDuration;
	timerBeforeAttack = beforeAttack;
}

void ICharacter::Update()
{
	if (!IsAlive())
		return;

	if (timerAttackCooldown < attackCooldown)
		timerAttackCooldown += TimeManager::GetDeltaTime();

	if (timerAttackDuration < attackDuration)
	{
		timerAttackDuration += TimeManager::GetDeltaTime();

		if (timerAttackDuration > attackDuration)
		{
			StopAttack();
			timerAttackCooldown = 0.0f;
		}
	}

	if (timerBeforeAttack < beforeAttack)
	{
		timerBeforeAttack += TimeManager::GetDeltaTime();
		if (timerBeforeAttack > beforeAttack)
			Attack();
	}

	if (asTakenDamage)
	{
		if (material)
			material->color = Vec4::Lerp(damageColor, baseColor, invulnerabilityTimer / invulnerabilityFrame);
		invulnerabilityTimer += TimeManager::GetDeltaTime();
		if (invulnerabilityTimer >= invulnerabilityFrame)
		{
			asTakenDamage = false;
			invulnerabilityTimer = 0.0f;
			if (material)
				material->color = baseColor;
		}
	}
}

bool ICharacter::IsAttacking() const
{
	return attackCollision->IsActive() || timerBeforeAttack < beforeAttack;
}

bool ICharacter::Damage(int _damage)
{
	if (asTakenDamage)
		return false;

	if (IsAlive())
	{
		life -= _damage;
		if (IsAlive())
		{
			asTakenDamage = true;
			if (lifeFont)
				lifeFont->ChangeText(std::to_string(life));
			audioBroadcaster->Play("damage.wav");
		}
		else
		{
			OnDeath();
			animation->Play(deathAnimName);
			audioBroadcaster->Play("death.wav");
			std::vector<BoxCollision*> boxCollisions = GetGameObject().GetComponents<BoxCollision>();
			for (BoxCollision* boxCollision : boxCollisions)
				boxCollision->Deactivate();
 			rb->Deactivate();
		}
	}
	else
		return false;

	return true;
}

void ICharacter::NeedToAttack()
{
	if (animation->IsFinish(attackAnimName) || !animation->IsPlaying(attackAnimName))
	{
		timerBeforeAttack = 0.0f;
		if (animation)
			animation->Play(attackAnimName);
	}
}

void ICharacter::Attack()
{
	audioBroadcaster->Play("attack.wav");
	attackCollision->Activate();
	timerAttackDuration = 0.0f;
}

void ICharacter::StopAttack()
{
	attackCollision->Deactivate();
}