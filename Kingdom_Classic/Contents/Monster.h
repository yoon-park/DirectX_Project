#pragma once
#include "MovingActor.h"

class AMonster : public AMovingActor
{
	GENERATED_BODY(AMovingActor)

public:
	AMonster();
	~AMonster();

	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	UStateManager State;

	int GetCoinNum()
	{
		return CoinNum;
	}

	void AddCoin()
	{
		CoinNum += 1;
	}

	void SubCoin()
	{
		CoinNum -= 1;
	}

protected:
	USpriteRenderer* Renderer;
	UCollision* Collision;

	int CoinNum = 0;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void ChangeDir(EEngineDir _Dir) override;

private:

};