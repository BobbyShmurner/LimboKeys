#include "key_manager.hpp"

#include <algorithm>

std::vector<KeyManager*> KeyManager::m_Loaded;
KeyManager* KeyManager::m_Current = nullptr;

KeyManager::KeyManager(const char* path, bool setAsCurrent, bool unloadCurrent) {
	m_Loaded.push_back(this);

	if (setAsCurrent) {
		setCurrent(this, unloadCurrent);
	}
}

KeyManager::~KeyManager() {
	for (Key* key : m_Keys) {
		delete key;
	}

	m_Loaded.erase(std::find(m_Loaded.begin(), m_Loaded.end(), this));
	if (m_Current == this) {
		m_Current = nullptr;
	}
}

void KeyManager::setCurrent(KeyManager* manager, bool unloadCurrent) {
	if (unloadCurrent && m_Current) {
		delete m_Current;
	}

	m_Current = manager;
}

void KeyManager::addKey(Key* key) {
	m_Keys.push_back(key);
}

void KeyManager::removeKey(Key* key) {
	m_Keys.erase(std::find(m_Keys.begin(), m_Keys.end(), key));
	delete key;
}

void KeyManager::render() {
	for (Key* key : m_Keys) {
		key->render();
	}
}

void KeyManager::initKeyWindows() {
	for (Key* key : m_Keys) {
		key->initWindow();
	}
}

void KeyManager::setKeyVisibility(bool visible) {
	for (Key* key : m_Keys) {
		key->setVisibility(visible);
	}
}

void KeyManager::setKeyDecoration(bool show) {
	for (Key* key : m_Keys) {
		key->setDecoration(show);
	}
}