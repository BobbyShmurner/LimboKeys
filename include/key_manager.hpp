#pragma once

#include <vector>

#include "key.hpp"

class KeyManager{
friend class Key;

private:
	static std::vector<KeyManager*> m_Loaded;
	static KeyManager* m_Current;

	std::vector<Key*> m_Keys;
	
	void addKey(Key* key);
public:
	static inline KeyManager* current() { return m_Current; }

	KeyManager(const char* path, bool setAsCurrent = true, bool unloadCurrent = true);
	~KeyManager();

	void removeKey(Key* key);

	void render();
	void initKeyWindows();
	void setKeyVisibility(bool visible);

	static void setCurrent(KeyManager* manager, bool unloadCurrent = true);
	inline void setCurrent(bool unloadCurrent = true) { KeyManager::setCurrent(this, unloadCurrent); }

	const std::vector<Key*>& keys() const { return m_Keys; }
};