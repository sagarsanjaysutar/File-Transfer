#include "inc/Sony.h"

Sony::Sony(QObject* parent = nullptr) : QObject(parent){ cout << "Sony constructor called.\n"; }

Sony::~Sony(){ cout << "Destructor called.\n"; }