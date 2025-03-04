// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolMagazine.h"

APistolMagazine::APistolMagazine()
{
    ItemName = EItemType::PISTOL_BULLET;
    ItemDescription = "A magazine filled with pistol bullets.";
    AmmoAmount = 15; 
    AmmoType = EItemType::PISTOL_BULLET;
}

