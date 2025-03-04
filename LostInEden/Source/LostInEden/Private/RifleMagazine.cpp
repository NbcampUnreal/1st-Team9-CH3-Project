// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleMagazine.h"

ARifleMagazine::ARifleMagazine()
{
    ItemName = EItemType::RIFLE_BULLET;
    ItemDescription = "A magazine filled with rifle bullets.";
    AmmoAmount = 30; 
    AmmoType = EItemType::RIFLE_BULLET; 
}
