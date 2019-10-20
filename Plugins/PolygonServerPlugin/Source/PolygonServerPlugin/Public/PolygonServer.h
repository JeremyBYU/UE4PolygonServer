// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <inttypes.h>

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "npy.h"
#include "ScrictMode.hpp"
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#undef NOUSER

#undef FLOAT
#undef check
#include "rpc/server.h"
//TODO: HACK: UE4 defines macro with stupid names like "check" that conflicts with msgpack library
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "PolygonServer.generated.h"

struct DPCommand {
	double lifetime = 0.0;
	std::vector<double> shell;
	std::vector<std::vector<double>> holes;
	std::array<int,3> color{ {255, 0, 0} };
	MSGPACK_DEFINE_MAP(lifetime, shell, holes, color);

	DPCommand()
	{}
};

struct PointCloud
{
	// this map will hold the class mapping values (string label of class -> interger value)
	std::vector<float> points;
	// this holds the points received from the point cloud
	std::unordered_map<std::string, int> class_mapping;
};

/**
 *
 */
UCLASS()
class POLYGONSERVERPLUGIN_API APolygonServer : public ATriggerVolume
{

	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Cloud Generation")
		float resolution = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Cloud Generation", META = (Name = "Show Trace"))
		bool showTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Cloud Generation", META = (Name = "Record Classes"))
		bool recordClasses = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Cloud Generation", META = (Name = "Save Directory"))
		FString saveDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point Cloud Generation", META = (Name = "Enabled"))
		bool enabled = true;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DrawPolygon(const DPCommand &cmd);
	void TimerElapsed();

private:
	std::unique_ptr<rpc::server> server;


};

