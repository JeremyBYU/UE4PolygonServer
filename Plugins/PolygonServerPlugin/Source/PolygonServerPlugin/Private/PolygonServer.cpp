// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#include "PolygonServer.h"
#include "DrawDebugHelpers.h"




// Called when the game starts or when spawned
void APolygonServer::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	// Timer put in because of paranio....
	if (enabled)
	{
		CreateServer();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("PolygonServer disabled."));
	}
}

void APolygonServer::DrawLinearRing(const std::vector<double> &ring, const std::array<int, 3> &color, const double &lifetime, const double &thickness)
{
	FColor color_f(color[0], color[1], color[2]);
	auto vertices = ring.size() / 3;
	if (vertices >= 3 && ring.size() % 3 == 0)
	{
		for (auto i = 0; i < vertices - 1; i += 1)
		{
			auto ii = 3 * i;
			auto nii = 3 * (i + 1);
			FVector start(ring[ii], ring[ii + 1], ring[ii + 2]);
			FVector end(ring[nii], ring[nii + 1], ring[nii + 2]);
			DrawDebugLine(GetWorld(), start, end, color_f, false, lifetime, 0, thickness);
		}
		// Close the Polygon
		auto ii = 3 * (vertices - 1);
		auto nii = 0;
		FVector start(ring[ii], ring[ii + 1], ring[ii + 2]);
		FVector end(ring[nii], ring[nii + 1], ring[nii + 2]);
		DrawDebugLine(GetWorld(), start, end, color_f, false, lifetime, 0, thickness);

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Incorrect number of 3D points for polygon: %f "), ring.size());
	}

}


void APolygonServer::DrawPolygon(const DPCommand &cmd)
{

	DrawLinearRing(cmd.shell, cmd.shell_color, cmd.lifetime, cmd.thickness);
	auto num_holes = cmd.holes.size();
	for (auto i = 0; i < num_holes; i++)
	{
		DrawLinearRing(cmd.holes[i], cmd.hole_color, cmd.lifetime, cmd.thickness);
	}
}

void APolygonServer::CreateServer()
{
	UE_LOG(LogTemp, Display, TEXT("Trying to start server"));
	std::string serverIP_std = std::string(TCHAR_TO_UTF8(*serverIP));
	server.reset(new rpc::server(serverIP_std, port));
	server->bind("ping", [&](const std::string& msg) -> void {
		UE_LOG(LogTemp, Display, TEXT("ping received"));
	});
	server->bind("drawPolygon", [&](const DPCommand& cmd) -> void {
		UE_LOG(LogTemp, Display, TEXT("polygon command received"));
		DrawPolygon(cmd);
	});


	server->async_run(2);   //2 threads
}
