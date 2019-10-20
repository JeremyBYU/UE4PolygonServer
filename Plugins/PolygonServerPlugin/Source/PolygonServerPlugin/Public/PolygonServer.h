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
STRICT_MODE_ON
#include "PolygonServer.generated.h"


struct DPCommand {
	double lifetime = 0.0;
	std::vector<double> shell;
	std::vector<std::vector<double>> holes;
	std::array<int,3> shell_color{ {0, 255, 0} };
	std::array<int, 3> hole_color{ {255, 0, 0} };
	double thickness = 1.0;
	MSGPACK_DEFINE_MAP(lifetime, shell, holes, shell_color, hole_color, thickness);

	DPCommand()
	{}
};

/**
 *
 */
UCLASS()
class POLYGONSERVERPLUGIN_API APolygonServer : public ATriggerVolume
{

	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polygon Server", META = (Name = "Enabled"))
		bool enabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polygon Server", META = (Name = "Server IP"))
		FString serverIP = "127.0.0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polygon Server", META = (Name = "Port"))
		int port = 3000;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DrawPolygon(const DPCommand &cmd);
	void DrawLinearRing(const std::vector<double> &ring, const std::array<int, 3> &color, const double &lifetime, const double &thickness);
	void CreateServer();

private:
	std::unique_ptr<rpc::server> server;


};

