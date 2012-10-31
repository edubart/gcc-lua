/*
 * Lua plugin for the GNU Compiler Collection.
 * Copyright © 2012 Peter Colberg.
 * For conditions of distribution and use, see copyright notice in LICENSE.
 */

typedef int electron;
typedef int muon;
enum tau { TAU = 1 };

void electron_neutrino(void);
void muon_neutrino(void) {}
void tau_neutrino(void) {}

int positron;
extern int anti_muon;
int anti_tau;
