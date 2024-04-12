#pragma once
#include "SceneComponent.h"

#include "EngineInputLayout.h"
#include "EngineMesh.h"
#include "EngineMaterial.h"
#include "EngineShaderResources.h"
#include "Camera.h"

class UEngineShaderResources;

class URenderer : public USceneComponent, public std::enable_shared_from_this<URenderer>
{
	GENERATED_BODY(USceneComponent)

	friend ULevel;

public:
	URenderer();
	~URenderer();

	URenderer(const URenderer& _Other) = delete;
	URenderer(URenderer&& _Other) noexcept = delete;
	URenderer& operator=(const URenderer& _Other) = delete;
	URenderer& operator=(URenderer&& _Other) noexcept = delete;

	std::shared_ptr<UEngineShaderResources> Resources;

	std::shared_ptr<UEngineMesh> GetMesh()
	{
		return Mesh;
	}

	std::shared_ptr<UEngineMaterial> GetMaterial()
	{
		return Material;
	}

	void SetMesh(std::string_view _Name);
	void SetMaterial(std::string_view _Name);

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order) override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void ResCopy(UEngineShader* _Shader);

private:
	std::shared_ptr<UEngineInputLayout> Layout;
	std::shared_ptr<UEngineMesh> Mesh;
	std::shared_ptr<UEngineMaterial> Material;

	void RenderingTransformUpdate(std::shared_ptr<UCamera> _Camera);
	void Render(float _DeltaTime);
};