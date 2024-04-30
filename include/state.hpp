#pragma once

#include "state_macros.hpp"

class State {
public:
	bool state_changed_update_now = false;

	bool running = true;
	bool spinKeys = true;

	float rotation = 0.0f;
	float revealAmount = 1.0f;
	float speedX = 0.25f;
	float speedY = 0.25f;
	float amplitudeX = 0.6f;
	float amplitudeY = 0.6f;
	float freqX = 1.0f;
	float freqY = 1.0f;

	STATE_CREATE_NOTIFY(bool, showKeys) = true;
	STATE_CREATE_NOTIFY(bool, decorateKeyWindows) = false;

	static State* instance() {
		static State* s = new State();
		return s;
	}

	static void exit(const char* msg, int code = 1) {
		fprintf(code == 0 ? stdout : stderr, "%s\n", msg);
		instance()->running = false;
	}
};