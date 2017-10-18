#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <float.h>

#include "components.h"
#include "Manifold.h"
#include "AutisticCmath.h"

void isCollidingCC(Manifold &man);
void isCollidingCP(Manifold &man);
void isCollidingPC(Manifold &man);
void isCollidingPP(Manifold &man);
