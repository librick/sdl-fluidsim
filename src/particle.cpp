#include <cmath>
#include "particle.h"
#include "../lib/fluidsim/fluidsim.h"
#include <iostream>

float randF() {
    return rand() / double(RAND_MAX);
}

Particle::Particle() {
    this->posX = randF()*N;
    this->posY = randF()*N;
    this->angle = randF()*2*M_PI;
    this->angularVelocity = randF()*2;
    this->velocityX = (randF()*2-1.0f)*0.3;
    this->velocityY = (randF()*2-1.0f)*0.3;
}
int Particle::getX() { return int(floor(this->posX)) % N; }
int Particle::getY() { return int(floor(this->posY)) % N; }
void Particle::update() {
    this->angle += this->angularVelocity;
    this->posX = this->posX + this->velocityX;
    this->posY = this->posY + this->velocityY;
}
