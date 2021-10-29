#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "GameObject.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/RigidBody.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/ProgressBar.hpp"
#include "Game/Enemy.hpp"
#include <imgui.h>
#include "Game/Player.hpp"


#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(GameObject* _gameobject, std::string _name)
	: ICharacter(_gameobject, _name)
{

}

void Player::OnAddComponent()
{
	ICharacter::OnAddComponent();
	camera = GetGameObject().AddComponent<Camera>();
	ProgressBar* progressBar = GetGameObject().AddComponent<ProgressBar>();
	progressBar->currentValue = (float*)&life;
	progressBar->maxValue = (float*)&maxLife;
	progressBar->SetName("LifeBar");
	progressBar = GetGameObject().AddComponent<ProgressBar>();
	progressBar->currentValue = (float*)&currentXp;
	progressBar->maxValue = (float*)&maxXp;
	progressBar->SetName("XpBar");
	GetGameObject().SetTag("Player");
}

void Player::OnContact(Object* _other, class ShapeCollision* _triggerShape)
{
	if (!_other->GetTag().compare("Ground"))
	{
		isJumping = false;
	}
}

void Player::Begin()
{
	attackAnimName = "Player Attack.fbx";
	walkAnimName = "Player Walk.fbx";
	idleAnimName = "Player Idle.fbx";
	deathAnimName = "Player Dying.fbx";
	camera = GetGameObject().GetComponent<Camera>();
	camera->SetPosition({ 0.0f, 12.5f, -25.0f });
	camera->SetTarget({ 0.0f, 7.0f, 0.0f });
	ProgressBar* progressBar = GetGameObject().GetComponentByName<ProgressBar>("XpBar");
	progressBar->currentValue = (float*)&currentXp;
	progressBar->maxValue = (float*)&maxXp;
	progressBar = GetGameObject().GetComponentByName<ProgressBar>("LifeBar");
	progressBar->currentValue = (float*)&life;
	progressBar->maxValue = (float*)&maxLife;
	maxXp = level * 10;
	attackDamage += attackDamageGain * (level - 1);
	maxLife += lifeGain * (level - 1);
	ICharacter::Begin();
	timerDashCooldown = dashCooldown;
	timerDashDuration = dashDuration;
}

void Player::Update()
{
	if (!IsAlive())
		return;

	if (timerDashCooldown < dashCooldown)
		timerDashCooldown += TimeManager::GetDeltaTime();

	if (timerDashDuration < dashDuration)
	{
		timerDashDuration += TimeManager::GetDeltaTime();

		if (timerDashDuration > dashDuration)
		{
			isDashing = false;
			timerDashCooldown = 0.0f;
		}
	}

	ICharacter::Update();

	if (InputManager::GetKeyPressed(E_KEYS::LSHIFT))
		speed = 8;
	else
		speed = 3;

	Vec2 offset = InputManager::GetCursorOffsetFromLastFrame();

	Quaternion camRotY = Quaternion(Vec3{ 0.0f, -offset.x * camSensitivity, 0.0f });
	camera->SetPosition(camRotY.RotateVector(camera->GetPosition()));

	if (totalAngle < -M_PI_4 && offset.y > 0 || totalAngle > M_PI_4 && offset.y < 0)
		return;

	Vec3 right = (-camera->GetPosition().Cross(Vec3::up)).Normalized();
	Quaternion camRotX = Quaternion::AngleAxis(right, -offset.y * camSensitivity / 30.f);

	totalAngle += -offset.y * camSensitivity / 30.f;
	camera->SetPosition(camRotX.RotateVector((camera->GetPosition() - GetGameObject().WorldPosition()) + GetGameObject().WorldPosition()));

	Vec3 cameraForward = camera->GetTarget() - camera->GetPosition();
	cameraForward.y = camera->GetPosition().y;
	cameraForward.Normalize();
	Vec3 cameraRight = cameraForward.Cross(Vec3::up).Normalized();

	Vec3 direction;

	if (InputManager::GetKeyPressed(E_KEYS::W))
		direction += cameraForward;
	if (InputManager::GetKeyPressed(E_KEYS::S))
		direction -= cameraForward;
	if (InputManager::GetKeyPressed(E_KEYS::D))
		direction += cameraRight;
	if (InputManager::GetKeyPressed(E_KEYS::A))
		direction -= cameraRight;

	if (InputManager::GetKeyPressedOneTime(E_KEYS::LCTRL) && !isDashing)
	{
		animation->Play("Player Roulade.fbx");
		isDashing = true;
		timerDashDuration = 0.0f;
		StopAttack();

		if (direction.GetSquaredMagnitude() < 0.01f)
			direction = GetGameObject().Forward();

		dashDirection = direction;
	}

	if (isDashing)
		direction = direction.GetSquaredMagnitude() > 0.01f ? direction : dashDirection;

	Vec3 linearVelocity = rb->GetLinearVelocity();

	if (direction.GetSquaredMagnitude() > 0.01f && !IsAttacking())
	{
		float angleToDirection = 0.25f * GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

		GetGameObject().RotateY((angleToDirection / M_PI) * 180.0f);

		if (!isDashing)
		{
			if (speed < 8)
			{
				if (animation && !animation->IsPlaying("Player Walking.fbx"))
					animation->Play("Player Walking.fbx");
			}
			else if (animation && !animation->IsPlaying("Player Run.fbx"))
				animation->Play("Player Run.fbx");

		}
		rb->SetLinearVelocity(GetGameObject().Forward() * TimeManager::GetDeltaTime() * (isDashing ? dashSpeed : speed) * 100.0f + (linearVelocity.y * Vec3::up));
	}

	else
	{
		if (linearVelocity.x * linearVelocity.x > 0.1f || linearVelocity.z * linearVelocity.z > 0.1f)
			rb->SetLinearVelocity(linearVelocity.y * Vec3::up);

		if (animation && (animation->IsFinish("Player Attack.fbx") || animation->IsFinish("Player Roulade.fbx") || animation->IsPlaying("Player Walking.fbx") || animation->IsPlaying("Player Run.fbx")))
			animation->Play("Player Idle.fbx");
	}

	if (InputManager::GetKeyPressed(E_KEYS::SPACEBAR) && !isJumping)
	{
		isJumping = true;
		rb->SetLinearVelocity(Vec3(0.0f, TimeManager::GetDeltaTime() * speed * 100.0f, 0.0f));
	}

	if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && timerAttackCooldown >= attackCooldown && timerAttackDuration >= attackDuration && timerBeforeAttack >= beforeAttack)
	{
		NeedToAttack();
		rb->SetLinearVelocity(linearVelocity.y * Vec3::up);
	}
}

void Player::ManageXp(const Enemy& _enemyKilled)
{
	currentXp += _enemyKilled.xpGiven;
	if (currentXp >= maxXp)
	{
		level++;
		currentXp -= maxXp;
		maxXp = level * 10;
		attackDamage += attackDamageGain;
		maxLife += lifeGain;
		life = maxLife;
	}
}

void Player::Editor()
{
	ICharacter::Editor();
	ImGui::Text("Level : ");
	ImGui::SameLine();
	ImGui::DragInt("", &level);
	ImGui::Text("Max Xp : %d", maxXp);
	ImGui::Text("Current Xp : %d", currentXp);
	ImGui::DragInt("Roulade speed", &dashSpeed, 1.0f, 0);
}