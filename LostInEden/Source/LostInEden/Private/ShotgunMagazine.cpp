// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunMagazine.h"

AShotgunMagazine::AShotgunMagazine()
{
    ItemName = EItemType::SHOTGUN_BULLET;
    ItemDescription = "A pack of shotgun shells.";
    AmmoAmount = 20; 
    AmmoType = EItemType::SHOTGUN_BULLET; 
}