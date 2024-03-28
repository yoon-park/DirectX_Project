#pragma once

template<typename ResType>
class UEngineResources
{
public:
	UEngineResources() {}
	~UEngineResources() {}

	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;

protected:

private:
	std::map<std::string, ResType*> Resources;
};

