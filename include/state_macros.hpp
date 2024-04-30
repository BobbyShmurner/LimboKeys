#pragma once

#define STATE_CREATE_NOTIFY(type, ident) \
	bool ident##_Changed = false; \
	type ident

#define STATE_IMGUI_PARMS(state, ...) \
	state __VA_OPT__(,) __VA_ARGS__)) { \
	state##_Changed = true; \
	}((void)0

#define STATE_NOTIFY_IMGUI if (

#define STATE_CHANGED(ident) \
	(false) {} \
	\
	State::instance()->state_changed_update_now = ident##_Changed; \
	\
	if (ident##_Changed) { \
		ident##_Changed = false; \
	} \
	\
	if (State::instance()->state_changed_update_now)
