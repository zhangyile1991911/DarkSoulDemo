#pragma once

#include "Engine/DataTable.h"
#include "EMontageAction.h"

#include "FMontageAction.generated.h"

USTRUCT(BlueprintType)
struct FMontageAction : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	EMontageAction Action = EMontageAction::None;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	UAnimMontage* Animation = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	float CostStamina = 5.0f;
};
