#pragma once

class State {
public:
	bool showKeys = true;
	bool running = true;
	float revealAmount = 1.0f;
	float speedX = 0.25f;
	float speedY = 0.25f;
	float amplitudeX = 0.6f;
	float amplitudeY = 0.6f;
	float freqX = 1.0f;
	float freqY = 1.0f;

	static State* instance() {
		static State* s = new State();
		return s;
	}
};